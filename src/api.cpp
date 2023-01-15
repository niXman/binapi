
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
#include <charconv>
#include <queue>
#include <type_traits>
#include <iostream>

#include <openssl/hmac.h>
#include <openssl/sha.h>

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

std::string sign(const std::string timestamp, const std::string method, const std::string requestPath, const std::string data, const std::string secret) {
    std::string prehash = timestamp + method + requestPath + data;
    //std::cout << "\nprehash: " << prehash << "\n" << std::endl;
    return base64_encode(hmacSHA256(secret, prehash));
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
        m_ssl_ctx{boost::asio::ssl::context::tlsv13},
        m_resolver{m_ioctx}
    {}

    using val_type = boost::variant<std::size_t, const char *>;
    using kv_type = std::pair<const char *, val_type>;
    using init_list_type = std::initializer_list<kv_type>;
    bool signed_request = false;

    template<
         typename CB
        ,typename Args = typename boost::callable_traits::args<CB>::type
        ,typename R = typename std::tuple_element<3, Args>::type
    >
    api::result<R>
    post(bool _signed, const char *target, boost::beast::http::verb action, std::string data, CB cb) {
        static_assert(std::tuple_size<Args>::value == 4, "callback signature is wrong!");

        auto is_html = [](const char *str) -> bool {
            return std::strstr(str, "<HTML>")
                || std::strstr(str, "<HEAD>")
                || std::strstr(str, "<BODY>")
            ;
        };

        std::string starget = target;
        
        bool get_delete = 
            action == boost::beast::http::verb::get ||
            action == boost::beast::http::verb::delete_
        ;
        if ( get_delete && !data.empty() ) {
            starget += data;
            data.clear();
        }

        signed_request = _signed;

        api::result<R> res{};
        if ( !cb ) {
            try {
                api::result<std::string> r = sync_post(starget.c_str(), action, std::move(data));
                if ( !r.v.empty() && is_html(r.v.c_str()) ) {
                    r.errmsg = std::move(r.v);
                } else {
                    simdjson::padded_string json = simdjson::padded_string(r.v);
                    simdjson::ondemand::parser parser;
                    simdjson::ondemand::document doc;
                    auto error = parser.iterate(json).get(doc);

                    if (error) {
                        std::cerr << error << std::endl;
                        res.reply.clear();

                        return res;
                    }

                    if (bg_api::rest::is_api_error(doc)) {
                        auto error = bg_api::rest::construct_error(doc);
                        res.ec = error.first;
                        __MAKE_ERRMSG(res, error.second)
                        res.reply.clear();

                        return res;
                    } else {
                        res.v = R::construct(doc);
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
        req.version(11);
        req.method(action);

        req.target(target);
        req.set(boost::beast::http::field::host, m_host);
        req.set(boost::beast::http::field::user_agent, m_client_api_string);
        req.set(boost::beast::http::field::accept, "*/*");
        if (signed_request) {
            std::string timestamp = std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
            std::string signature = sign(timestamp, boost::beast::http::to_string(action).to_string(), target, data, m_sk);
            req.set("ACCESS-KEY", m_pk);
            req.set("ACCESS-SIGN", signature);
            req.set("ACCESS-PASSPHRASE", m_passphrase);
            req.set("ACCESS-TIMESTAMP", timestamp);
        }
        req.set("locale", m_locale);
        req.set(boost::beast::http::field::content_type, "application/json");
        if ( action != boost::beast::http::verb::get ) {
            req.body() = std::move(data);
            req.set(boost::beast::http::field::content_length, std::to_string(req.body().length()));
        }

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
        //std::cout << "async_post(): data=" << data << std::endl;
        //std::cout << "async_post(): target=" << target << std::endl;

        auto req = std::make_unique<request_type>();
        
        req->version(11);
        req->method(action);

        req->target(target);
        req->set(boost::beast::http::field::host, m_host);
        req->set(boost::beast::http::field::user_agent, m_client_api_string);
        req->set(boost::beast::http::field::accept, "*/*");
        if (signed_request) {
            std::string timestamp = std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
            std::string signature = sign(timestamp, boost::beast::http::to_string(action).to_string(), target, data, m_sk);
            req->set("ACCESS-KEY", m_pk);
            req->set("ACCESS-SIGN", signature);
            req->set("ACCESS-PASSPHRASE", m_passphrase);
            req->set("ACCESS-TIMESTAMP", timestamp);
        }
        req->set("locale", m_locale);
        req->set(boost::beast::http::field::content_type, "application/json");
        if ( action != boost::beast::http::verb::get ) {
            req->body() = std::move(data);
            req->set(boost::beast::http::field::content_length, std::to_string(req->body().length()));
        }

        //std::cout << "Request Headers:" << std::endl;
        //for (auto &h : req->base()) {
        //    std::cout << "Field: " << h.name() << "/text: " << h.name_string() << ", Value: " << h.value() << std::endl;
        //}
        //std::cout << "Request Body:" << req->body() << std::endl;
        //std::cout << std::endl;

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
        
        //std::cout << "Response Headers:" << std::endl;
        //for (auto &h : resp->base()) {
        //    std::cout << "Field: " << h.name() << "/text: " << h.name_string() << ", Value: " << h.value() << std::endl;
        //}

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
            //std::cout << "process_reply(): ec=" << ec << std::endl;
            //std::cout << "process_reply(): errmsg=" << errmsg << std::endl;
            //std::cout << "process_reply(): body=" << body << std::endl;
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

/*************************************************************************************************/
/****************************************SPOT ENDPOINTS*******************************************/
/*************************************************************************************************/

api::result<server_time_t> api::getServerTime(server_time_cb cb) {
    return pimpl->post(false, "/api/spot/v1/public/time", boost::beast::http::verb::get, "", std::move(cb));
}

/*************************************************************************************************/

api::result<coin_list_t> api::getCoinList(coin_list_cb cb) {
    return pimpl->post(false, "/api/spot/v1/public/currencies", boost::beast::http::verb::get, "", std::move(cb));
}

/*************************************************************************************************/

api::result<symbols_t> api::getSymbols(symbols_cb cb) {
    return pimpl->post(false, "/api/spot/v1/public/products", boost::beast::http::verb::get, "", std::move(cb));
}

/*************************************************************************************************/

api::result<symbol_t> api::getSymbol(const char* symbol, symbol_cb cb) {
    std::string params = "?symbol=";
    params += symbol;

    return pimpl->post(false, "/api/spot/v1/public/product", boost::beast::http::verb::get, params, std::move(cb));
}

/*************************************************************************************************/

api::result<spot_ticker_t> api::getSpotTicker(const char* symbol, spot_ticker_cb cb) {
    std::string params = "?symbol=";
    params += symbol;

    return pimpl->post(false, "/api/spot/v1/market/ticker", boost::beast::http::verb::get, params, std::move(cb));
}

/*************************************************************************************************/

api::result<spot_tickers_t> api::getSpotTickers(spot_tickers_cb cb) {
    return pimpl->post(false, "/api/spot/v1/market/tickers", boost::beast::http::verb::get, "", std::move(cb));
}

/*************************************************************************************************/

api::result<trades_t> api::getSpotTrades(const char* symbol, spot_trades_cb cb, uint16_t limit) {
    std::string params = "?symbol=";
    params += symbol;
    if (limit) {
        params += "&limit=";
        params += std::to_string(limit);
    }

    return pimpl->post(false, "/api/spot/v1/market/fills", boost::beast::http::verb::get, params, std::move(cb));
}

/*************************************************************************************************/

api::result<candles_t> api::getSpotCandles(const char* symbol, _candle_gran period, candles_cb cb, std::size_t startTime, std::size_t endTime, uint16_t limit) {
    std::string params = "?symbol=";
    params += symbol;
    params += "&period=";
    params += candle_gran_to_string(period);
    if (startTime) {
        params += "&start_time=";
        params += std::to_string(startTime);
    }
    if (endTime) {
        params += "&end_time=";
        params += std::to_string(endTime);
    }
    if (limit) {
        params += "&limit=";
        params += std::to_string(limit);
    }

    return pimpl->post(false, "/api/spot/v1/market/candles", boost::beast::http::verb::get, params, std::move(cb));
}

/*************************************************************************************************/

api::result<depth_t> api::getSpotDepth(const char* symbol, depth_cb cb, uint16_t limit) {
    std::string params = "?symbol=";
    params += symbol;
    if (limit) {
        params += "&limit=";
        params += std::to_string(limit);
    }

    return pimpl->post(false, "/api/spot/v1/market/depth", boost::beast::http::verb::get, params, std::move(cb));
}

/*************************************************************************************************/

api::result<transfer_res_t> api::transfer(_from_to_type from, _from_to_type to, double_type amount, const char* coin, transfer_cb cb, const char* clientOid) {
    std::string params = "{\"fromType\":\"";
    params += from_to_type_to_string(from);
    params += "\",\"toType\":\"";
    params += from_to_type_to_string(to);
    params += "\",\"amount\":\"";
    params += amount.str();
    params += "\",\"coin\":\"";
    params += coin;
    if (clientOid) {
        params += "\",\"clientOid\":\"";
        params += clientOid;
    }
    params += "\"}";


    return pimpl->post(true, "/api/spot/v1/wallet/transfer", boost::beast::http::verb::post, params, std::move(cb));
}

/*************************************************************************************************/

api::result<address_t> api::getAddress(const char* coin, address_cb cb, const char* chain) {
    std::string params = "?coin=";
    params += coin;
    if (chain) {
        params += "&chain=";
        params += chain;
    }

    return pimpl->post(true, "/api/spot/v1/wallet/deposit-address", boost::beast::http::verb::get, params, std::move(cb));
}

/*************************************************************************************************/

api::result<withdraw_res_t> api::withdraw(const char* coin, const char* address, const char* chain, double_type amount, withdraw_cb cb, const char* tag, const char* remark, const char* clientOid) {
    std::string params = "{\"coin\":\"";
    params += coin;
    params += "\",\"address\":\"";
    params += address;
    params += "\",\"chain\":\"";
    params += chain;
    params += "\",\"amount\":\"";
    params += amount.str();
    if (tag) {
        params += "\",\"tag\":\"";
        params += tag;
    }
    if (remark) {
        params += "\",\"remark\":\"";
        params += remark;
    }
    if (clientOid) {
        params += "\",\"clientOid\":\"";
        params += clientOid;
    }
    params += "\"}";

    return pimpl->post(true, "/api/spot/v1/wallet/withdraw", boost::beast::http::verb::post, params, std::move(cb));
}

/*************************************************************************************************/

api::result<withdraw_res_t> api::innerWithdraw(const char* coin, const char* toUid, double_type amount, withdraw_cb cb, const char* clientOid) {
    std::string params = "{\"coin\":\"";
    params += coin;
    params += "\",\"toUid\":\"";
    params += toUid;
    params += "\",\"amount\":\"";
    params += amount.str();
    if (clientOid) {
        params += "\",\"clientOid\":\"";
        params += clientOid;
    }
    params += "\"}";


    return pimpl->post(true, "/api/spot/v1/wallet/withdrawal-inner", boost::beast::http::verb::post, params, std::move(cb));
}

/*************************************************************************************************/

api::result<deposit_withdrawals_t> api::withdrawList(const char* coin, std::size_t startTime, std::size_t endTime, withdraw_list_cb cb, std::size_t pageNo, uint8_t pageSize) {
    std::string params = "?coin=";
    params += coin;
    params += "&startTime=";
    params += std::to_string(startTime);
    params += "&endTime=";
    params += std::to_string(endTime);
    if (pageNo) {
        params += "&pageNo=";
        params += std::to_string(pageNo);
    }
    if (pageSize) {
        params += "&pageSize=";
        params += std::to_string(pageSize);
    }

    return pimpl->post(true, "/api/spot/v1/wallet/withdrawal-list", boost::beast::http::verb::get, params, std::move(cb));
}

/*************************************************************************************************/

api::result<deposit_withdrawals_t> api::depositList(const char* coin, std::size_t startTime, std::size_t endTime, deposit_list_cb cb, std::size_t pageNo, uint8_t pageSize) {
    std::string params = "?coin=";
    params += coin;
    params += "&startTime=";
    params += std::to_string(startTime);
    params += "&endTime=";
    params += std::to_string(endTime);
    if (pageNo) {
        params += "&pageNo=";
        params += std::to_string(pageNo);
    }
    if (pageSize) {
        params += "&pageSize=";
        params += std::to_string(pageSize);
    }

    return pimpl->post(true, "/api/spot/v1/wallet/deposit-list", boost::beast::http::verb::get, params, std::move(cb));
}

/*************************************************************************************************/

api::result<apikey_t> api::getApiKeyInfo(apikey_cb cb) {
    return pimpl->post(true, "/api/spot/v1/account/getInfo", boost::beast::http::verb::get, "", std::move(cb));
}

/*************************************************************************************************/

api::result<spot_account_t> api::getSpotAccount(const char* coin, account_cb cb) {
    std::string params = "?coin=";
    params += coin;

    return pimpl->post(true, "/api/spot/v1/account/assets", boost::beast::http::verb::get, params, std::move(cb));
}

/*************************************************************************************************/

api::result<bills_t> api::getBills(std::size_t coinId, _group_type groupType, _biz_type bizType, bills_cb cb, std::size_t after, std::size_t before, uint16_t limit) {
    std::string params = "{\"coinId\":\"";
    params += std::to_string(coinId);
    params += "\",\"groupType\":\"";
    params += group_type_to_string(groupType);
    params += "\",\"bizType\":\"";
    params += biz_type_to_string(bizType);
    if (after) {
        params += "\",\"after\":\"";
        params += std::to_string(after);
    }
    if (before) {
        params += "\",\"before\":\"";
        params += std::to_string(before);
    }
    if (limit) {
        params += "\",\"limit\":\"";
        params += std::to_string(limit);
    }
    params += "\"}";

    return pimpl->post(true, "/api/spot/v1/account/bills", boost::beast::http::verb::post, params, std::move(cb));
}

/*************************************************************************************************/

api::result<transfers_t> api::getTransferList(std::size_t coinId, const char* fromType, transfers_cb cb, std::size_t after, std::size_t before, uint16_t limit) {
    std::string params = "?coinId=";
    params += std::to_string(coinId);
    params += "&fromType=";
    params += fromType;
    if (after) {
        params += "&after=";
        params += std::to_string(after);
    }
    if (before) {
        params += "&before=";
        params += std::to_string(before);
    }
    if (limit) {
        params += "&limit=";
        params += std::to_string(limit);
    }

    return pimpl->post(true, "/api/spot/v1/account/transferRecords", boost::beast::http::verb::get, params, std::move(cb));
}

/*************************************************************************************************/

api::result<spot_order_res_t> api::placeSpotOrder(const char* symbol, _side side, _order_type type, _force force, double_type quantity, place_order_cb cb, double_type price, const char* clientOid) {
    std::string params = "{\"symbol\":\"";
    params += symbol;
    params += "\",\"side\":\"";
    params += side_to_string(side);
    params += "\",\"type\":\"";
    params += order_type_to_string(type);
    params += "\",\"force\":\"";
    params += force_to_string(force);
    params += "\",\"quantity\":\"";
    params += quantity.str();
    if (price) {
        params += "\",\"price\":\"";
        params += price.str();
    }
    if (clientOid) {
        params += "\",\"clientOid\":\"";
        params += clientOid;
    }
    params += "\"}";

    return pimpl->post(true, "/api/spot/v1/trade/orders", boost::beast::http::verb::post, params, std::move(cb));
}

/*************************************************************************************************/

api::result<spot_orders_res_t> api::placeSpotOrders(const char* symbol, std::vector<_side> sides, std::vector<_order_type> types, std::vector<_force> forces, std::vector<double_type> quantities, std::vector<double_type> prices, std::vector<std::string> clientOids, place_orders_cb cb) {
    assert(sides.size() == types.size() && sides.size() == forces.size() && sides.size() == quantities.size() && sides.size() == prices.size() && sides.size() == clientOids.size());
    std::string params = "{\"symbol\":\"";
    params += symbol;
    params += "\",\"orderList\":[";
    for (std::size_t i = 0; i < sides.size(); ++i) {
        params += "{\"side\":\"";
        params += side_to_string(sides[i]);
        params += "\",\"type\":\"";
        params += order_type_to_string(types[i]);
        params += "\",\"force\":\"";
        params += force_to_string(forces[i]);
        params += "\",\"quantity\":\"";
        params += quantities[i].str();
        if (prices[i]) {
            params += "\",\"price\":\"";
            params += prices[i].str();
        }
        if (!clientOids[i].empty()) {
            params += "\",\"clientOid\":\"";
            params += clientOids[i];
        }
        params += "\"}";
        if (i != sides.size() - 1) {
            params += ",";
        }
    }
    params += "]}";

    return pimpl->post(true, "/api/spot/v1/trade/batch-orders", boost::beast::http::verb::post, params, std::move(cb));
}

/*************************************************************************************************/

api::result<spot_cancel_res_t> api::cancelSpotOrder(const char* symbol, const char* orderId, cancel_order_cb cb) {
    std::string params = "{\"symbol\":\"";
    params += symbol;
    params += "\",\"orderId\":\"";
    params += orderId;
    params += "\"}";

    return pimpl->post(true, "/api/spot/v1/trade/cancel-order", boost::beast::http::verb::post, params, std::move(cb));
}


/*************************************************************************************************/

api::result<spot_cancel_res_t> api::cancelSpotOrders(const char* symbol, std::vector<std::string> orderIds, cancel_orders_cb cb) {
    std::string params = "{\"symbol\":\"";
    params += symbol;
    params += "\",\"orderIds\":[";
    for (std::size_t i = 0; i < orderIds.size(); ++i) {
        params += "\"";
        params += orderIds[i];
        params += "\"";
        if (i != orderIds.size() - 1) {
            params += ",";
        }
    }
    params += "]}";

    return pimpl->post(true, "/api/spot/v1/trade/cancel-batch-orders", boost::beast::http::verb::post, params, std::move(cb));
}

/*************************************************************************************************/

api::result<spot_orders_t> api::getOrderDetails(const char* symbol, const char* orderId, order_detail_cb cb, const char* clientOrderId) {
    std::string params = "{\"symbol\":\"";
    params += symbol;
    params += "\",\"orderId\":\"";
    params += orderId;
    if (clientOrderId) {
        params += "\",\"clientOid\":\"";
        params += clientOrderId;
    }
    params += "\"}";

    return pimpl->post(true, "/api/spot/v1/trade/orderInfo", boost::beast::http::verb::post, params, std::move(cb));
}

/*************************************************************************************************/

api::result<spot_orders_t> api::getOrderList(const char* symbol, order_list_cb cb) {
    std::string params = "";
    if (symbol) {
        params += "{\"symbol\":\"";
        params += symbol;
        params += "\"}";
    } 

    return pimpl->post(true, "/api/spot/v1/trade/open-orders", boost::beast::http::verb::get, params, std::move(cb));
}

/*************************************************************************************************/

api::result<spot_orders_t> api::orderHistory(const char* symbol, order_history_cb cb, std::size_t after, std::size_t before, uint32_t limit) {
    std::string params = "{\"symbol\":\"";
    params += symbol;
    params += "\"";
    if (after) {
        params += "\",\"after\":";
        params += std::to_string(after);
    }
    if (before) {
        params += ",\"before\":";
        params += std::to_string(before);
    }
    if (limit) {
        params += ",\"limit\":";
        params += std::to_string(limit);
    }
    params += "}";


    return pimpl->post(true, "/api/spot/v1/trade/history", boost::beast::http::verb::post, params, std::move(cb));
}

/*************************************************************************************************/

api::result<transactions_t> api::getTransactions(const char* symbol, transactions_cb cb, const char* orderId, std::size_t after, std::size_t before, uint32_t limit) {
    std::string params = "{\"symbol\":\"";
    params += symbol;
    params += "\"";
    if (orderId) {
        params += ",\"orderId\":\"";
        params += orderId;
    }
    if (after) {
        params += ",\"after\":";
        params += std::to_string(after);
    }
    if (before) {
        params += ",\"before\":";
        params += std::to_string(before);
    }
    if (limit) {
        params += ",\"limit\":";
        params += std::to_string(limit);
    }
    params += "}";

    return pimpl->post(true, "/api/spot/v1/trade/fills", boost::beast::http::verb::post, params, std::move(cb));
}

/*************************************************************************************************/

api::result<spot_plan_order_res_t> api::placeSpotPlanOrder(const char* symbol, _side side, double_type triggerPrice, double_type size, _trigger_type triggerType, _order_type orderType, place_spot_plan_order_cb cb, double_type executePrice, const char* clientOid, _force force) {
    std::string params = "{\"symbol\":\"";
    params += symbol;
    params += "\",\"side\":\"";
    params += side_to_string(side);
    params += "\",\"triggerPrice\":\"";
    params += triggerPrice.str();
    if (executePrice) {
        params += "\",\"executePrice\":\"";
        params += executePrice.str();
    }
    params += "\",\"size\":\"";
    params += size.str();
    params += "\",\"triggerType\":\"";
    params += trigger_type_to_string(triggerType);
    params += "\",\"orderType\":\"";
    params += order_type_to_string(orderType);
    if (clientOid) {
        params += "\",\"clientOid\":\"";
        params += clientOid;
    }
    params += "\",\"force\":\"";
    params += force_to_string(force);
    params += "\"}";

    return pimpl->post(true, "/api/spot/v1/trade/plan-order", boost::beast::http::verb::post, params, std::move(cb));
}

/*************************************************************************************************/

api::result<spot_plan_order_res_t> api::modifySpotPlanOrder(const char* orderId, double_type triggerPrice, _order_type orderType, modify_spot_plan_order_cb cb, double_type executePrice, double_type size) {
    std::string params = "{\"orderId\":\"";
    params += orderId;
    params += "\",\"triggerPrice\":\"";
    params += triggerPrice.str();
    if (executePrice) {
        params += "\",\"executePrice\":\"";
        params += executePrice.str();
    }
    if (size) {
        params += "\",\"size\":\"";
        params += size.str();
    }
    params += "\",\"orderType\":\"";
    params += order_type_to_string(orderType);
    params += "\"}";

    return pimpl->post(true, "/api/spot/v1/trade/modifyPlan", boost::beast::http::verb::post, params, std::move(cb));
}

/*************************************************************************************************/

api::result<cancel_spot_plan_order_res_t> api::cancelSpotPlanOrder(const char* orderId, cancel_spot_plan_order_cb cb) {
    std::string params = "{\"orderId\":\"";
    params += orderId;
    params += "\"}";

    return pimpl->post(true, "/api/spot/v1/trade/cancelPlan", boost::beast::http::verb::post, params, std::move(cb));
}

/*************************************************************************************************/

api::result<spot_plan_orders_t> api::getSpotPlanOrders(const char* symbol, spot_plan_orders_cb cb, uint16_t pageSize, const char* lastEndId) {
    std::string params = "{\"symbol\":\"";
    params += symbol;
    params += "\"";
    if (pageSize) {
        params += ",\"pageSize\":";
        params += std::to_string(pageSize);
    }
    if (lastEndId) {
        params += ",\"lastEndId\":\"";
        params += lastEndId;
    }
    params += "}";

    return pimpl->post(true, "/api/spot/v1/trade/plan-orders", boost::beast::http::verb::post, params, std::move(cb));
}

/*************************************************************************************************/

api::result<spot_plan_orders_t> api::getSpotPlanOrderHistory(const char* symbol, std::size_t startTime, std::size_t endTime, spot_plan_orders_cb cb, uint16_t pageSize, const char* lastEndId) {
    std::string params = "{\"symbol\":\"";
    params += symbol;
    params += "\"";
    if (pageSize) {
        params += ",\"pageSize\":";
        params += std::to_string(pageSize);
    }
    if (lastEndId) {
        params += ",\"lastEndId\":\"";
        params += lastEndId;
    }
    params += ",\"startTime\":";
    params += std::to_string(startTime);
    params += ",\"endTime\":";
    params += std::to_string(endTime);
    params += "}";

    return pimpl->post(true, "/api/spot/v1/trade/plan-orders-history", boost::beast::http::verb::post, params, std::move(cb));
}

/*************************************************************************************************/
/**************************************FUTURES ENDPOINTS******************************************/
/*************************************************************************************************/

api::result<contracts_t> api::getContracts(_product_type productType, contracts_cb cb) {
    std::string params = "?productType=";
    params += product_type_to_string(productType);

    return pimpl->post(false, "/api/mix/v1/market/contracts", boost::beast::http::verb::get, params, std::move(cb));
}

/*************************************************************************************************/

api::result<depth_t> api::getFuturesDepth(const char* symbol, depth_cb cb, uint8_t limit) {
    std::string params = "?symbol=";
    params += symbol;
    if (limit == 5 || limit == 15 || limit == 50 || limit == 100) {
        params += "&limit=";
        params += std::to_string(limit);
    }

    return pimpl->post(false, "/api/mix/v1/market/depth", boost::beast::http::verb::get, params, std::move(cb));
}

/*************************************************************************************************/

api::result<futures_ticker_t> api::getFuturesTicker(const char* symbol, futures_ticker_cb cb) {
    std::string params = "?symbol=";
    params += symbol;

    return pimpl->post(false, "/api/mix/v1/market/ticker", boost::beast::http::verb::get, params, std::move(cb));
}

/*************************************************************************************************/

api::result<futures_tickers_t> api::getFuturesTickers(_product_type productType, futures_tickers_cb cb) {
    std::string params = "?productType=";
    params += product_type_to_string(productType);

    return pimpl->post(false, "/api/mix/v1/market/tickers", boost::beast::http::verb::get, "", std::move(cb));
}

/*************************************************************************************************/

api::result<fills_t> api::getFills(const char* symbol, fills_cb cb, uint16_t limit) {
    std::string params = "?symbol=";
    params += symbol;
    if (limit) {
        params += "&limit=";
        params += std::to_string(limit);
    }

    return pimpl->post(false, "/api/mix/v1/market/fills", boost::beast::http::verb::get, params, std::move(cb));
}

/*************************************************************************************************/

api::result<candles_t> api::getFuturesCandles(const char* symbol, _candle_gran granularity, std::size_t startTime, std::size_t endTime, candles_cb cb) {
    std::string params = "?symbol=";
    params += symbol;
    params += "&granularity=";
    params += candle_gran_to_string(granularity);
    params += "&startTime=";
    params += std::to_string(startTime);
    params += "&endTime=";
    params += std::to_string(endTime);

    return pimpl->post(false, "/api/mix/v1/market/candles", boost::beast::http::verb::get, params, std::move(cb));
}

/*************************************************************************************************/

api::result<index_t> api::getFuturesIndexPrice(const char* symbol, index_cb cb) {
    std::string params = "?symbol=";
    params += symbol;

    return pimpl->post(false, "/api/mix/v1/market/index", boost::beast::http::verb::get, params, std::move(cb));
}

/*************************************************************************************************/

api::result<funding_t> api::getNextFundingTime(const char* symbol, funding_cb cb) {
    std::string params = "?symbol=";
    params += symbol;

    return pimpl->post(false, "/api/mix/v1/market/funding-time", boost::beast::http::verb::get, params, std::move(cb));
}

/*************************************************************************************************/

api::result<fundings_t> api::getHistoryFundRate(const char* symbol, history_funding_cb cb, uint16_t pageSize, uint16_t pageNo, bool nextPage) {
    std::string params = "?symbol=";
    params += symbol;
    if (pageSize) {
        params += "&pageSize=";
        params += std::to_string(pageSize);
    }
    if (pageNo) {
        params += "&pageNo=";
        params += std::to_string(pageNo);
    }
    if (nextPage) {
        params += "&nextPage=";
        params += nextPage ? "true" : "false";
    }

    return pimpl->post(false, "/api/mix/v1/market/history-fundRate", boost::beast::http::verb::get, params, std::move(cb));
}

/*************************************************************************************************/

api::result<funding_t> api::getFundRate(const char* symbol, funding_cb cb) {
    std::string params = "?symbol=";
    params += symbol;

    return pimpl->post(false, "/api/mix/v1/market/current-fundRate", boost::beast::http::verb::get, params, std::move(cb));
}

/*************************************************************************************************/

api::result<open_interest_t> api::getOpenInterest(const char* symbol, open_interest_cb cb) {
    std::string params = "?symbol=";
    params += symbol;

    return pimpl->post(false, "/api/mix/v1/market/open-interest", boost::beast::http::verb::get, params, std::move(cb));
}

/*************************************************************************************************/

api::result<mark_price_t> api::getMarkPrice(const char* symbol, mark_price_cb cb) {
    std::string params = "?symbol=";
    params += symbol;

    return pimpl->post(false, "/api/mix/v1/market/mark-price", boost::beast::http::verb::get, params, std::move(cb));
}

/*************************************************************************************************/

api::result<leverage_t> api::getLeverage(const char* symbol, leverage_cb cb) {
    std::string params = "?symbol=";
    params += symbol;

    return pimpl->post(false, "/api/mix/v1/position/symbol-leverage", boost::beast::http::verb::get, params, std::move(cb));
}

/*************************************************************************************************/



} // ns rest
} // ns bg_api
