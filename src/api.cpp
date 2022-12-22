
// ----------------------------------------------------------------------------
//                              Apache License
//                        Version 2.0, January 2004
//                     http://www.apache.org/licenses/
//
// This file is part of bg_api(https://github.com/patrickk33/bg_api) project. A fork of 
// niXman's binapi(https://github.com/niXman/binapi) project.
//
// Copyright (c) 2019-2021 niXman (github dot nixman dog pm.me). All rights reserved.
// ----------------------------------------------------------------------------

#include <bg_api/api.hpp>
#include <bg_api/invoker.hpp>
#include <bg_api/errors.hpp>

#include <boost/preprocessor.hpp>
#include <boost/callable_traits.hpp>
#include <boost/variant.hpp>

#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/algorithm/string.hpp>

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
#include <bg_api/flatjson.hpp>

namespace bg_api {
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

std::string hmacSHA256(std::string key, std::string msg) {
    std::array<unsigned char, EVP_MAX_MD_SIZE> hash;
    unsigned int hashLen;
    HMAC(EVP_sha256(),
        key.data(),
        static_cast<int>(key.size()),
        reinterpret_cast<unsigned char const*>(msg.data()),
        static_cast<int>(msg.size()),
        hash.data(),
        &hashLen);
    return std::string(reinterpret_cast<char const*>(hash.data()), hashLen);
}

std::string base64_encode(std::string &msg) {
    using namespace boost::archive::iterators;
    using It = base64_from_binary<transform_width<std::string::const_iterator, 6, 8>>;
    auto tmp = std::string(It(begin(msg)), It(end(msg)));
    return tmp.append((3 - msg.size() % 3) % 3, '=');
}

/*************************************************************************************************/

struct api::impl {
    impl(
         boost::asio::io_context &ioctx,
        std::string host,
        std::string port,
        std::string pk,
        std::string passphrase,
        std::string sk,
        std::size_t timeout,
        std::string locale,
        std::string client_api_string
    ) :
        m_ioctx{ioctx},
        m_host{std::move(host)},
        m_port{std::move(port)},
        m_pk{std::move(pk)},
        m_passphrase{std::move(passphrase)},
        m_sk{std::move(sk)},
        m_timeout{timeout},
        m_locale{std::move(locale)},
        m_client_api_string{std::move(client_api_string)},
        m_write_in_process{},
        m_async_requests{},
        m_ssl_ctx{boost::asio::ssl::context::sslv23_client},
        m_resolver{m_ioctx}
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

                    if ( json.is_object() && bg_api::rest::is_api_error(json) ) {
                        auto error = bg_api::rest::construct_error(json);
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
        req.set(boost::beast::http::field::content_type, "application/json");

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
        req->set(boost::beast::http::field::content_type, "application/json");

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
    const std::string m_passphrase;
    const std::string m_sk;
    const std::size_t m_timeout;
    const std::string m_locale;
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
     boost::asio::io_context &ioctx,
    std::string host,
    std::string port,
    std::string pk,
    std::string passphrase,
    std::string sk,
    std::size_t timeout,
    std::string locale,
    std::string client_api_string
)
    :pimpl{std::make_unique<impl>(
        ioctx,
        std::move(host),
        std::move(port),
        std::move(pk),
        std::move(passphrase),
        std::move(sk),
        timeout,
        std::move(locale),
        std::move(client_api_string)
    )}
{}

api::~api()
{}


} // ns rest
} // ns bg_api
