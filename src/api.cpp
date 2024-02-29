
// ----------------------------------------------------------------------------
//                              Apache License
//                        Version 2.0, January 2004
//                     http://www.apache.org/licenses/
//
// This file is part of binapi(https://github.com/niXman/binapi) project.
//
// Copyright (c) 2019-2021 niXman (github dot nixman dog pm.me). All rights reserved.
// ----------------------------------------------------------------------------

#include <binapi/api.hpp>
#include <binapi/invoker.hpp>
#include <binapi/errors.hpp>

#include <boost/preprocessor.hpp>
#include <boost/callable_traits.hpp>
#include <boost/variant.hpp>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>

#include <chrono>
#include <queue>
#include <type_traits>
#include <iostream>

#include <openssl/hmac.h>
#include <openssl/sha.h>
#include <binapi/flatjson.hpp>

namespace binapi {
namespace rest {

/*************************************************************************************************/

#define __CATCH_BLOCK_WRITES_TO_STDOUT

#ifndef __CATCH_BLOCK_WRITES_TO_STDOUT
#   define  __CATCH_BLOCK_WRITES_TO_STDOUT_EXPAND_EXPR(...)
#else
#   define  __CATCH_BLOCK_WRITES_TO_STDOUT_EXPAND_EXPR(...) __VA_ARGS__
#endif // __CATCH_BLOCK_WRITES_TO_STDOUT

#define __CATCH_BLOCK_WITH_USERCODE(os, exception, ...) \
    catch (const exception &ex) { \
        __CATCH_BLOCK_WRITES_TO_STDOUT_EXPAND_EXPR( \
            os << __MESSAGE("[" BOOST_PP_STRINGIZE(exception) "]: " << ex.what()) << std::endl; \
        ) \
        { BOOST_PP_EXPAND __VA_ARGS__; } \
    }

#define __CATCH_BLOCK_WITHOUT_USERCODE(os, exception, ...) \
    catch (const exception &ex) { \
        __CATCH_BLOCK_WRITES_TO_STDOUT_EXPAND_EXPR( \
            os << __MESSAGE("[" BOOST_PP_STRINGIZE(exception) "]: " << ex.what()) << std::endl; \
        ) \
    }

#define __CATCH_BLOCK_CB(unused0, data, elem) \
    BOOST_PP_IF( \
         BOOST_PP_GREATER(BOOST_PP_TUPLE_SIZE(elem), 1) \
        ,__CATCH_BLOCK_WITH_USERCODE \
        ,__CATCH_BLOCK_WITHOUT_USERCODE \
    )( \
         data \
        ,BOOST_PP_TUPLE_ELEM(0, elem) \
        ,BOOST_PP_TUPLE_POP_FRONT(elem) \
    )

#define __CATCH_BLOCK_WRAP_X(...) ((__VA_ARGS__)) __CATCH_BLOCK_WRAP_Y
#define __CATCH_BLOCK_WRAP_Y(...) ((__VA_ARGS__)) __CATCH_BLOCK_WRAP_X
#define __CATCH_BLOCK_WRAP_X0
#define __CATCH_BLOCK_WRAP_Y0

#define __CATCH_BLOCK(os, seq) \
    BOOST_PP_SEQ_FOR_EACH( \
         __CATCH_BLOCK_CB \
        ,os \
        ,BOOST_PP_CAT(__CATCH_BLOCK_WRAP_X seq, 0) \
    )

#define __TRY_BLOCK() \
    try

/*************************************************************************************************/

std::uint64_t get_current_ms_epoch() {
    return static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count());
}

std::string b2a_hex(const std::uint8_t *p, std::size_t n) {
    static const char hex[] = "0123456789abcdef";
    std::string res;
    res.reserve(n * 2);

    for ( auto end = p + n; p != end; ++p ) {
        const std::uint8_t v = (*p);
        res += hex[(v >> 4) & 0x0F];
        res += hex[v & 0x0F];
    }

    return res;
}

std::string hmac_sha256(const char *key, std::size_t klen, const char *data, std::size_t dlen) {
    std::uint8_t digest[EVP_MAX_MD_SIZE];
    std::uint32_t dilen{};

    auto p = ::HMAC(
         ::EVP_sha256()
        ,key
        ,klen
        ,(std::uint8_t *)data
        ,dlen
        ,digest
        ,&dilen
    );
    assert(p);

    return b2a_hex(digest, dilen);
}

// unused for now
bool verify_signature(const unsigned char* sig, std::size_t slen, const char* data, std::size_t dlen)
{
    bool result = true;
    auto pubkeyfile = BIO_new_file ( "testnet.pub.pem", "r" );
    auto vkey = PEM_read_bio_PUBKEY( pubkeyfile, nullptr, nullptr, nullptr);

    auto ctx = EVP_MD_CTX_new();
    auto md = ::EVP_sha256();
    assert(pubkeyfile && vkey && ctx && md);

    if ( 1 != ( EVP_DigestInit_ex ( ctx, md, nullptr ) &&
                EVP_DigestVerifyInit ( ctx, nullptr, md, nullptr, vkey ) &&
                EVP_DigestVerifyUpdate ( ctx, data, dlen ) &&
                EVP_DigestVerifyFinal ( ctx, sig, slen ) ) )
    {
        std::cerr << "EVP_DigestVerify* failed!" << std::endl;
        result = false;
    }

    if (pubkeyfile)
        BIO_free( pubkeyfile );
    if (ctx)
        EVP_MD_CTX_free( ctx );
    if (vkey)
        EVP_PKEY_free( vkey );

    return result;
}

// unused for now
std::string rsa_sha256(const char* privkeyfile, std::size_t /*pklen*/, const char *data, std::size_t dlen )
{
    static EVP_PKEY *pkey = nullptr;
    if ( !pkey ) {
         auto keybp = BIO_new_file ( privkeyfile, "r" );
         pkey = EVP_PKEY_new();
         pkey = PEM_read_bio_PrivateKey(keybp, nullptr, nullptr, nullptr);

         if ( keybp )
             BIO_free(keybp);
    }
    assert(pkey);

    auto mdctx = EVP_MD_CTX_new();
    std::size_t req = 0, slen = 0;
    if ( 1 != (EVP_DigestSignInit( mdctx, nullptr, ::EVP_sha256(), nullptr, pkey ) &&
               EVP_DigestSignUpdate( mdctx, data, dlen ) &&
               EVP_DigestSignFinal( mdctx, nullptr, &req )) )
    {
        std::cerr << "EVP_DigestSign* failed!" << std::endl;
        exit(1);
    }

    unsigned char* signature;
    slen = req;
    signature =  static_cast<unsigned char*> ( OPENSSL_malloc ( req ) );
    if ( 1 != EVP_DigestSignFinal ( mdctx, signature, &slen ) ) {
        std::cerr << "Digest Final (2) failed" << std::endl;
    }
    assert(slen == req);

    // Uncomment to verify if priv/pub keypairs are working together
    // assert( verify_signature(signature, slen, data, dlen ) );

    unsigned char encodedSig[512];
    int elen = EVP_EncodeBlock ( encodedSig, signature, slen );

    if ( mdctx )
        EVP_MD_CTX_free(mdctx);
    if ( signature )
        OPENSSL_free(signature);

    return std::string(reinterpret_cast<char*>(encodedSig), elen);
}

/*************************************************************************************************/

struct api::impl {
    impl(
         boost::asio::io_context &ioctx
        ,std::string host
        ,std::string port
        ,std::string pk
        ,std::string sk
        ,std::size_t timeout
        ,std::string client_api_string
    )
        :m_ioctx{ioctx}
        ,m_host{std::move(host)}
        ,m_port{std::move(port)}
        ,m_pk{std::move(pk)}
        ,m_sk{std::move(sk)}
        ,m_timeout{timeout}
        ,m_client_api_string{std::move(client_api_string)}
        ,m_write_in_process{}
        ,m_async_requests{}
        ,m_ssl_ctx{boost::asio::ssl::context::sslv23_client}
        ,m_resolver{m_ioctx}
    {}

    using val_type = boost::variant<std::size_t, const char *>;
    using kv_type = std::pair<const char *, val_type>;
    using init_list_type = std::initializer_list<kv_type>;

    template<
         typename CB
        ,typename Args = typename boost::callable_traits::args<CB>::type
        ,typename R = typename std::tuple_element<3, Args>::type
    >
    api::result<R>
    post(bool _signed, const char *target, boost::beast::http::verb action, const std::initializer_list<kv_type> &map, CB cb) {
        static_assert(std::tuple_size<Args>::value == 4, "callback signature is wrong!");

        auto is_valid_value = [](const val_type &v) -> bool {
            if ( const auto *p = boost::get<const char *>(&v) ) {
                return *p != nullptr;
            }
            if ( const auto *p = boost::get<std::size_t>(&v) ) {
                return *p != 0u;
            }

            assert(!"unreachable");

            return false;
        };

        auto to_string = [](char *buf, std::size_t bufsize, const val_type &v) -> const char* {
            if ( const auto *p = boost::get<const char *>(&v) ) {
                return *p;
            }
            if ( const auto *p = boost::get<std::size_t>(&v) ) {
                std::snprintf(buf, bufsize, "%zu", *p);

                return buf;
            }

            assert(!"unreachable");

            return buf;
        };

        auto is_html = [](const char *str) -> bool {
            return std::strstr(str, "<HTML>")
                || std::strstr(str, "<HEAD>")
                || std::strstr(str, "<BODY>")
            ;
        };

        std::string starget = target;
        std::string data;
        for ( const auto &it: map ) {
            if ( is_valid_value(it.second) ) {
                if ( !data.empty() ) {
                    data += "&";
                }
                data += it.first;
                data += "=";

                char buf[32];
                data += to_string(buf, sizeof(buf), it.second);
            }
        }

        if ( _signed ) {
            assert(!m_pk.empty() && !m_sk.empty());

            if ( !data.empty() ) {
                data += "&";
            }
            data += "timestamp=";
            char buf[32];
            data += to_string(buf, sizeof(buf), get_current_ms_epoch());

            data += "&recvWindow=";
            data += to_string(buf, sizeof(buf), m_timeout);

            std::string signature = hmac_sha256(
                 m_sk.c_str()
                ,m_sk.length()
                ,data.c_str()
                ,data.length()
            );

            data += "&signature=";
            data += signature;
        }

        bool get_delete =
            action == boost::beast::http::verb::get ||
            action == boost::beast::http::verb::delete_
        ;
        if ( get_delete && !data.empty() ) {
            starget += "?";
            starget += data;
            data.clear();
        }

        api::result<R> res{};
        if ( !cb ) {
            try {
                api::result<std::string> r = sync_post(starget.c_str(), action, std::move(data));
                if ( !r.v.empty() && is_html(r.v.c_str()) ) {
                    r.errmsg = std::move(r.v);
                } else {
                    std::string strbuf = std::move(r.v);
                    const flatjson::fjson json{strbuf.c_str(), strbuf.length()};
                    if ( json.error() != flatjson::FJ_EC_OK ) {
                        res.ec = json.error();
                        __MAKE_ERRMSG(res, json.error_string())
                        res.reply.clear();

                        return res;
                    }

                    if ( json.is_object() && binapi::rest::is_api_error(json) ) {
                        auto error = binapi::rest::construct_error(json);
                        res.ec = error.first;
                        __MAKE_ERRMSG(res, error.second)
                        res.reply.clear();

                        return res;
                    } else {
                        res.v = R::construct(json);
                    }
                }
            } catch (const std::exception &ex) {
                __MAKE_ERRMSG(res, ex.what())
            }

            return res;
        } else {
            using invoker_type = detail::invoker<typename boost::callable_traits::return_type<CB>::type, R, CB>;
            async_req_item item{
                 starget
                ,action
                ,std::move(data)
                ,std::make_shared<invoker_type>(std::move(cb))
            };
            m_async_requests.push(std::move(item));

            async_post();
        }

        return res;
    }

    api::result<std::string>
    sync_post(const char *target, boost::beast::http::verb action, std::string data) {
        api::result<std::string> res{};

        boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_stream(m_ioctx, m_ssl_ctx);

        if( !SSL_set_tlsext_host_name(ssl_stream.native_handle(), m_host.c_str()) ) {
            boost::system::error_code ec{static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category()};
            std::cerr << __MESSAGE("msg=" << ec.message()) << std::endl;

            __MAKE_ERRMSG(res, ec.message());
            return res;
        }

        boost::system::error_code ec;
        auto const results = m_resolver.resolve(m_host, m_port, ec);
        if ( ec ) {
            std::cerr << __MESSAGE("msg=" << ec.message()) << std::endl;

            __MAKE_ERRMSG(res, ec.message());
            return res;
        }

        boost::asio::connect(ssl_stream.next_layer(), results.begin(), results.end(), ec);
        if ( ec ) {
            std::cerr << __MESSAGE("msg=" << ec.message()) << std::endl;

            __MAKE_ERRMSG(res, ec.message());
            return res;
        }

        ssl_stream.handshake(boost::asio::ssl::stream_base::client, ec);
        if ( ec ) {
            std::cerr << __MESSAGE("msg=" << ec.message()) << std::endl;

            __MAKE_ERRMSG(res, ec.message());
            return res;
        }

        boost::beast::http::request<boost::beast::http::string_body> req;
        req.target(target);
        req.version(11);

        req.method(action);
        if ( action != boost::beast::http::verb::get ) {
            req.body() = std::move(data);
            req.set(boost::beast::http::field::content_length, std::to_string(req.body().length()));
        }

        req.insert("X-MBX-APIKEY", m_pk);
        req.set(boost::beast::http::field::host, m_host);
        req.set(boost::beast::http::field::user_agent, m_client_api_string);
        req.set(boost::beast::http::field::content_type, "application/x-www-form-urlencoded");

        boost::beast::http::write(ssl_stream, req, ec);
        if ( ec ) {
            std::cerr << __MESSAGE("msg=" << ec.message()) << std::endl;

            __MAKE_ERRMSG(res, ec.message());
            return res;
        }

        boost::beast::flat_buffer buffer;
        boost::beast::http::response<boost::beast::http::string_body> bres;

        boost::beast::http::read(ssl_stream, buffer, bres, ec);
        if ( ec ) {
            std::cerr << __MESSAGE("msg=" << ec.message()) << std::endl;

            __MAKE_ERRMSG(res, ec.message());
            return res;
        }

        res.v = std::move(bres.body());
//        std::cout << target << " REPLY:\n" << res.v << std::endl << std::endl;

        ssl_stream.shutdown(ec);

        return res;
    }

    using request_ptr = std::unique_ptr<boost::beast::http::request<boost::beast::http::string_body>>;
    using request_type = typename request_ptr::element_type;
    using response_ptr = std::unique_ptr<boost::beast::http::response<boost::beast::http::string_body>>;
    using response_type = typename response_ptr::element_type;
    using ssl_socket_ptr = std::unique_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>>;
    using ssl_socket_type = typename ssl_socket_ptr::element_type;

    void async_post() {
        if ( m_write_in_process ) {
            return;
        }

        m_write_in_process = true;

        auto &front = m_async_requests.front();
        auto action = front.action;
        std::string data = std::move(front.data);
        std::string target = front.target;
        //std::cout << "async_post(): target=" << target << std::endl;

        auto req = std::make_unique<request_type>();
        req->version(11);
        req->method(action);
        if ( action != boost::beast::http::verb::get ) {
            req->body() = std::move(data);
            req->set(boost::beast::http::field::content_length, std::to_string(req->body().length()));
        }

        req->target(target);
        req->insert("X-MBX-APIKEY", m_pk);
        req->set(boost::beast::http::field::host, m_host);
        req->set(boost::beast::http::field::user_agent, m_client_api_string);
        req->set(boost::beast::http::field::content_type, "application/x-www-form-urlencoded");

        //std::cout << target << " REQUEST:\n" << m_req << std::endl;

        // Look up the domain name
        m_resolver.async_resolve(
             m_host
            ,m_port
            ,[this, req=std::move(req)]
             (const boost::system::error_code &ec, boost::asio::ip::tcp::resolver::results_type res) mutable
             { on_resolve(ec, std::move(req), std::move(res)); }
        );
    }
    void on_resolve(
         const boost::system::error_code &ec
        ,request_ptr req
        ,boost::asio::ip::tcp::resolver::results_type results)
    {
        if ( ec ) {
            m_write_in_process = false;
            process_reply(__MAKE_FILELINE, ec.value(), ec.message(), std::string{});
            return;
        }

        ssl_socket_ptr ssl_socket = std::make_unique<ssl_socket_type>(m_ioctx, m_ssl_ctx);

        if(! SSL_set_tlsext_host_name(ssl_socket->native_handle(), m_host.c_str())) {
            boost::system::error_code ec2{static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category()};
            std::cerr << __MESSAGE("msg=" << ec2.message()) << std::endl;

            return;
        }

        auto sptr = ssl_socket.get();

        boost::asio::async_connect(
             sptr->next_layer()
            ,results.begin()
            ,results.end()
            ,[this, req=std::move(req), ssl_socket=std::move(ssl_socket)]
             (const boost::system::error_code &ec, auto) mutable
             { on_connect(ec, std::move(req), std::move(ssl_socket)); }
        );
    }
    void on_connect(
         const boost::system::error_code &ec
        ,request_ptr req
        ,ssl_socket_ptr ssl_socket)
    {
        if ( ec ) {
            m_write_in_process = false;
            process_reply(__MAKE_FILELINE, ec.value(), ec.message(), std::string{});
            return;
        }

        auto sptr = ssl_socket.get();

        // Perform the SSL handshake
        sptr->async_handshake(
             boost::asio::ssl::stream_base::client
            ,[this, req=std::move(req), ssl_socket=std::move(ssl_socket)]
             (const boost::system::error_code &ec) mutable
             { on_handshake(ec, std::move(req), std::move(ssl_socket)); }
        );
    }
    void on_handshake(
         const boost::system::error_code &ec
        ,request_ptr req
        ,ssl_socket_ptr ssl_socket)
    {
        if ( ec ) {
            m_write_in_process = false;
            process_reply(__MAKE_FILELINE, ec.value(), ec.message(), std::string{});
            return;
        }

        auto *request_ptr = req.get();
        auto *socket_ptr = ssl_socket.get();

        // Send the HTTP request to the remote host
        boost::beast::http::async_write(
            *socket_ptr
            ,*request_ptr
            ,[this, req=std::move(req), ssl_socket=std::move(ssl_socket)]
             (const boost::system::error_code &ec, std::size_t wr) mutable
             { on_write(ec, std::move(req), std::move(ssl_socket), wr); }
        );
    }
    void on_write(const boost::system::error_code &ec, request_ptr req, ssl_socket_ptr ssl_socket, std::size_t wr) {
        boost::ignore_unused(wr);
        boost::ignore_unused(req);

        if ( ec ) {
            m_write_in_process = false;
            process_reply(__MAKE_FILELINE, ec.value(), ec.message(), std::string{});
            return;
        }

        auto resp = std::make_unique<response_type>();
        auto *resp_ptr = resp.get();
        auto *socket_ptr = ssl_socket.get();

        // Receive the HTTP response
        boost::beast::http::async_read(
             *socket_ptr
            ,m_buffer
            ,*resp_ptr
            ,[this, resp=std::move(resp), ssl_socket=std::move(ssl_socket)]
             (const boost::system::error_code &ec, std::size_t rd) mutable
             { on_read(ec, std::move(resp), std::move(ssl_socket), rd); }
        );
    }
    void on_read(const boost::system::error_code &ec, response_ptr resp, ssl_socket_ptr ssl_socket, std::size_t rd) {
        boost::ignore_unused(rd);

        if ( ec ) {
            m_write_in_process = false;
            process_reply(__MAKE_FILELINE, ec.value(), ec.message(), std::string{});
            return;
        }

        auto *socket_ptr = ssl_socket.get();

        socket_ptr->async_shutdown(
            [this, resp=std::move(resp), ssl_socket=std::move(ssl_socket)]
            (const boost::system::error_code &ec) mutable
            { on_shutdown(ec, std::move(resp), std::move(ssl_socket)); }
        );
    }
    void on_shutdown(const boost::system::error_code &ec, response_ptr resp, ssl_socket_ptr ssl_socket) {
        boost::ignore_unused(ec);
        boost::ignore_unused(ssl_socket);

        std::string body = std::move(resp->body());
        process_reply(__MAKE_FILELINE, 0, std::string{}, std::move(body));

        m_write_in_process = false;

        if ( !m_async_requests.empty() ) {
            async_post();
        }
    }

    void process_reply(const char *fl, int ec, std::string errmsg, std::string body) {
        assert(!m_async_requests.empty());

        __TRY_BLOCK() {
            const auto item = std::move(m_async_requests.front());
            m_async_requests.pop();

            //std::cout << "process_reply(): target=" << item.target << std::endl;
            item.invoker->invoke(fl, ec, std::move(errmsg), body.c_str(), body.size());
        } __CATCH_BLOCK(
            std::cout,
            (std::exception)
        )
    }

    boost::asio::io_context &m_ioctx;
    const std::string m_host;
    const std::string m_port;
    const std::string m_pk;
    const std::string m_sk;
    const std::size_t m_timeout;
    const std::string m_client_api_string;

    bool m_write_in_process;
    struct async_req_item {
        std::string target;
        boost::beast::http::verb action;
        std::string data;
        detail::invoker_ptr invoker;
    };
    std::queue<async_req_item> m_async_requests;
    boost::asio::ssl::context m_ssl_ctx;
    boost::asio::ip::tcp::resolver m_resolver;
    boost::beast::flat_buffer m_buffer; // (Must persist between reads)
};

/*************************************************************************************************/

api::api(
     boost::asio::io_context &ioctx
    ,std::string host
    ,std::string port
    ,std::string pk
    ,std::string sk
    ,std::size_t timeout
    ,std::string client_api_string
)
    :pimpl{std::make_unique<impl>(
         ioctx
        ,std::move(host)
        ,std::move(port)
        ,std::move(pk)
        ,std::move(sk)
        ,timeout
        ,std::move(client_api_string)
    )}
{}

api::~api()
{}

/*************************************************************************************************/

api::result<ping_t> api::ping(ping_cb cb) {
    return pimpl->post(false, "/api/v3/ping", boost::beast::http::verb::get, {}, std::move(cb));
}

/*************************************************************************************************/

api::result<server_time_t> api::server_time(server_time_cb cb) {
    return pimpl->post(false, "/api/v3/time", boost::beast::http::verb::get, {}, std::move(cb));
}

/*************************************************************************************************/

api::result<prices_t::price_t> api::price(const char *symbol, price_cb cb) {
    const impl::init_list_type map = {
        {"symbol", symbol}
    };

    return pimpl->post(false, "/api/v3/ticker/price", boost::beast::http::verb::get, map, std::move(cb));
}

api::result<prices_t> api::prices(prices_cb cb) {
    return pimpl->post(false, "/api/v3/ticker/price", boost::beast::http::verb::get, {}, std::move(cb));
}

/*************************************************************************************************/

api::result<avg_price_t> api::avg_price(const char *symbol, avg_price_cb cb) {
    const impl::init_list_type map = {
        {"symbol", symbol}
    };

    return pimpl->post(false, "/api/v3/avgPrice", boost::beast::http::verb::get, map, std::move(cb));
}

/*************************************************************************************************/

api::result<_24hrs_tickers_t::_24hrs_ticker_t> api::_24hrs_ticker(const char *symbol, api::_24hrs_ticker_cb cb) {
    const impl::init_list_type map = {
        {"symbol", symbol}
    };

    return pimpl->post(false, "/api/v3/ticker/24hr", boost::beast::http::verb::get, map, std::move(cb));
}

api::result<_24hrs_tickers_t> api::_24hrs_tickers(api::_24hrs_tickers_cb cb) {
    return pimpl->post(false, "/api/v3/ticker/24hr", boost::beast::http::verb::get, {}, std::move(cb));
}

/*************************************************************************************************/

api::result<exchange_info_t> api::exchange_info(exchange_info_cb cb) {
    return pimpl->post(false, "/api/v3/exchangeInfo", boost::beast::http::verb::get, {}, std::move(cb));
}

api::result<exchange_info_t> api::exchange_info(const char *symbol, exchange_info_cb cb) {
    const impl::init_list_type map = {
         {"symbol", symbol}
    };

    return pimpl->post(false, "/api/v3/exchangeInfo", boost::beast::http::verb::get, map, std::move(cb));
}

api::result<exchange_info_t> api::exchange_info(const std::vector<std::string> &symbols, exchange_info_cb cb) {
    std::string symstr = "[";
    for ( auto it = symbols.begin(); it != symbols.end(); ++it) {
        symstr += "\"";
        symstr += *it;
        symstr += "\"";
        if ( std::next(it) != symbols.end()) {
            symstr += ",";
        }
    }
    symstr += "]";

    const impl::init_list_type map = {
         {"symbols", symstr.c_str()}
    };

    return pimpl->post(false, "/api/v3/exchangeInfo", boost::beast::http::verb::get, map, std::move(cb));
}

/*************************************************************************************************/

api::result<depths_t> api::depths(const char *symbol, std::size_t limit, depths_cb cb) {
    const impl::init_list_type map = {
         {"symbol", symbol}
        ,{"limit", limit}
    };

    return pimpl->post(false, "/api/v3/depth", boost::beast::http::verb::get, map, std::move(cb));
}

/*************************************************************************************************/

api::result<trades_t::trade_t> api::trade(const char *symbol, trade_cb cb) {
    const impl::init_list_type map = {
         {"symbol", symbol}
        ,{"limit", 1u}
    };

    return pimpl->post(false, "/api/v3/trades", boost::beast::http::verb::get, map, std::move(cb));
}

api::result<trades_t> api::trades(const char *symbol, std::size_t limit, trades_cb cb) {
    const impl::init_list_type map = {
         {"symbol", symbol}
        ,{"limit", limit}
    };

    return pimpl->post(false, "/api/v3/trades", boost::beast::http::verb::get, map, std::move(cb));
}

/*************************************************************************************************/

api::result<agg_trades_t::agg_trade_t> api::agg_trade(const char *symbol, agg_trade_cb cb) {
    const impl::init_list_type map = {
         {"symbol", symbol}
        ,{"limit", 1u}
    };

    return pimpl->post(false, "/api/v3/aggTrades", boost::beast::http::verb::get, map, std::move(cb));
}

api::result<agg_trades_t> api::agg_trades(const char *symbol, std::size_t limit, agg_trades_cb cb) {
    const impl::init_list_type map = {
         {"symbol", symbol}
        ,{"limit", limit}
    };

    return pimpl->post(false, "/api/v3/aggTrades", boost::beast::http::verb::get, map, std::move(cb));
}

/*************************************************************************************************/

api::result<klines_t> api::klines(const char *symbol, const char *interval, std::size_t limit, klines_cb cb) {
    const impl::init_list_type map = {
         {"symbol", symbol}
        ,{"limit", limit}
        ,{"interval", interval}
    };

    return pimpl->post(false, "/api/v3/klines", boost::beast::http::verb::get, map, std::move(cb));
}

/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/

api::result<account_info_t> api::account_info(account_info_cb cb) {
    return pimpl->post(true, "/api/v3/account", boost::beast::http::verb::get, {}, std::move(cb));
}

/*************************************************************************************************/

api::result<order_info_t> api::order_info(const char *symbol, std::size_t orderid, const char *client_orderid, order_info_cb cb) {
    const impl::init_list_type map = {
         {"symbol", symbol}
        ,{"orderId", orderid}
        ,{"origClientOrderId", client_orderid}
    };

    return pimpl->post(true, "/api/v3/order", boost::beast::http::verb::get, map, std::move(cb));
}

/*************************************************************************************************/

api::result<orders_info_t> api::open_orders(const char *symbol, open_orders_cb cb) {
    const impl::init_list_type map = {
        {"symbol", symbol}
    };

    return pimpl->post(true, "/api/v3/openOrders", boost::beast::http::verb::get, map, std::move(cb));
}

/*************************************************************************************************/

api::result<orders_info_t> api::all_orders(
     const char *symbol
    ,std::size_t orderid
    ,std::size_t start_time
    ,std::size_t end_time
    ,std::size_t limit
    ,all_orders_cb cb
) {
    const impl::init_list_type map = {
         {"symbol", symbol}
        ,{"orderId", orderid}
        ,{"startTime", start_time}
        ,{"endTime", end_time}
        ,{"limit", limit}
    };

    return pimpl->post(true, "/api/v3/allOrders", boost::beast::http::verb::get, map, std::move(cb));
}

/*************************************************************************************************/

api::result<new_order_resp_type>
api::new_order(
     const char *symbol
    ,const e_side side
    ,const e_type type
    ,const e_time time
    ,const e_trade_resp_type resp
    ,const char *amount
    ,const char *price
    ,const char *client_order_id
    ,const char *stop_price
    ,const char *iceberg_amount
    ,new_order_cb cb
) {
    const char *side_str = e_side_to_string(side);
    assert(side_str);

    const char *type_str = e_type_to_string(type);
    assert(type_str);

    const char *time_str = type == e_type::market
        ? nullptr
        : e_time_to_string(time)
    ;

    const char *response_type = e_trade_resp_type_to_string(resp);
    assert(response_type);

    const impl::init_list_type map = {
         {"symbol", symbol}
        ,{"side", side_str}
        ,{"type", type_str}
        ,{"timeInForce", time_str}
        ,{"quantity", amount}
        ,{"price", price}
        ,{"newClientOrderId", client_order_id}
        ,{"stopPrice", stop_price}
        ,{"icebergQty", iceberg_amount}
        ,{"newOrderRespType", response_type}
    };

    return pimpl->post(true, "/api/v3/order", boost::beast::http::verb::post, map, std::move(cb));
}

/*************************************************************************************************/

api::result<new_order_resp_type>
api::new_test_order(
     const char *symbol
    ,const e_side side
    ,const e_type type
    ,const e_time time
    ,const e_trade_resp_type resp
    ,const char *amount
    ,const char *price
    ,const char *client_order_id
    ,const char *stop_price
    ,const char *iceberg_amount
    ,new_order_cb cb
) {
    const char *side_str = e_side_to_string(side);
    assert(side_str);

    const char *type_str = e_type_to_string(type);
    assert(type_str);

    const char *time_str = type == e_type::market ? nullptr : e_time_to_string(time);

    const char *response_type = e_trade_resp_type_to_string(resp);
    assert(response_type);

    const impl::init_list_type map = {
         {"symbol", symbol}
        ,{"side", side_str}
        ,{"type", type_str}
        ,{"timeInForce", time_str}
        ,{"quantity", amount}
        ,{"price", price}
        ,{"newClientOrderId", client_order_id}
        ,{"stopPrice", stop_price}
        ,{"icebergQty", iceberg_amount}
        ,{"newOrderRespType", response_type}
    };

    return pimpl->post(true, "/api/v3/order/test", boost::beast::http::verb::post, map, std::move(cb));
}

/*************************************************************************************************/

api::result<cancel_order_info_t> api::cancel_order(
     const char *symbol
    ,std::size_t order_id
    ,const char *client_order_id
    ,const char *new_client_order_id
    ,cancel_order_cb cb
) {
    const impl::init_list_type map = {
         {"symbol", symbol}
        ,{"orderId", order_id}
        ,{"origClientOrderId", client_order_id}
        ,{"newClientOrderId", new_client_order_id}
    };

    return pimpl->post(true, "/api/v3/order", boost::beast::http::verb::delete_, map, std::move(cb));
}

/*************************************************************************************************/

api::result<my_trades_info_t> api::my_trades(
     const char *symbol
    ,std::size_t start_time
    ,std::size_t end_time
    ,std::size_t from_id
    ,std::size_t limit
    ,my_trades_cb cb
) {
    const impl::init_list_type map = {
         {"symbol", symbol}
        ,{"startTime", start_time}
        ,{"endTime", end_time}
        ,{"fromId", from_id}
        ,{"limit", limit}
    };

    return pimpl->post(true, "/api/v3/myTrades", boost::beast::http::verb::get, map, std::move(cb));
}

/*************************************************************************************************/

api::result<start_user_data_stream_t> api::start_user_data_stream(start_user_data_stream_cb cb) {
    return pimpl->post(false, "/api/v3/userDataStream", boost::beast::http::verb::post, {}, std::move(cb));
}

/*************************************************************************************************/

api::result<ping_user_data_stream_t> api::ping_user_data_stream(const char *listen_key, ping_user_data_stream_cb cb) {
    const impl::init_list_type map = {
        {"listenKey", listen_key}
    };

    return pimpl->post(false, "/api/v3/userDataStream", boost::beast::http::verb::put, map, std::move(cb));
}

/*************************************************************************************************/

api::result<close_user_data_stream_t> api::close_user_data_stream(const char *listen_key, close_user_data_stream_cb cb) {
    const impl::init_list_type map = {
        {"listenKey", listen_key}
    };

    return pimpl->post(false, "/api/v3/userDataStream", boost::beast::http::verb::delete_, map, std::move(cb));
}

/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/

} // ns rest
} // ns binapi
