
// ----------------------------------------------------------------------------
//                              Apache License
//                        Version 2.0, January 2004
//                     http://www.apache.org/licenses/
//
// This file is part of binapi(https://github.com/niXman/binapi) project.
//
// Copyright (c) 2019-2020 niXman (github dot nixman dog pm.me). All rights reserved.
// ----------------------------------------------------------------------------

#include <binapi/api.hpp>
#include <binapi/invoker.hpp>

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

// TODO: re-do without using openssl
#include <openssl/hmac.h>
#include <openssl/sha.h>

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
        ,typename R = typename std::tuple_element<3, typename boost::callable_traits::args<CB>::type>::type
    >
    api::result<R>
    post(bool _signed, const char *target, const char *action, const std::initializer_list<kv_type> &map, CB cb) {
        static_assert(std::tuple_size<typename boost::callable_traits::args<CB>::type>::value == 4, "");

        api::result<R> res{};

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

                char buf[1024];
                data += to_string(buf, sizeof(buf), it.second);
            }
        }

        if ( _signed ) {
            if ( !data.empty() ) {
                data += "&";
            }
            data += "timestamp=";
            char buf[1024];
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

        auto get_delete =
            std::strcmp(action, "GET") == 0 ||
            std::strcmp(action, "DELETE") == 0
        ;
        if ( get_delete && !data.empty() ) {
            starget += "?";
            starget += data;
            data.clear();
        }

        if ( !cb ) {
            try {
                auto r = sync_post(starget.c_str(), action, std::move(data));
                if ( !r.v.empty() && is_html(r.v.c_str()) ) {
                    r.errmsg = std::move(r.v);
                }
                if ( !r ) {
                    res.errmsg = std::move(r.errmsg);
                } else {
                    std::string strbuf = std::move(r.v);
                    if ( strbuf.find("\"code\":") != std::string::npos && strbuf.find("\"msg\":") != std::string::npos ) {
                        res.errmsg = strbuf;
                        res.reply = std::move(strbuf);
                    } else {
                        res.v = R::parse(strbuf.c_str(), strbuf.length());
                    }
                }
            } catch (const std::exception &ex) {
                __MAKE_ERRMSG(res, ex.what())
            } catch (...) {
                __MAKE_ERRMSG(res, "unknown exception")
            }
        } else {
            async_req_item item;
            item.target = starget;
            item.action = action;
            item.data = std::move(data);
            using invoker_type = detail::invoker<typename boost::callable_traits::return_type<CB>::type, R, CB>;
            item.invoker = std::make_shared<invoker_type>(std::move(cb));
            m_async_requests.push(std::move(item));
            async_post();
        }

        return res;
    }

    api::result<std::string>
    sync_post(const char *target, const char *action, std::string data) {
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
        if ( 0 == std::strcmp(action, "GET") ) {
            req.method(boost::beast::http::verb::get);
        } else if ( 0 == std::strcmp(action, "POST") ) {
            req.method(boost::beast::http::verb::post);
            auto body_size = data.size();
            req.body() = std::move(data);
            req.set(boost::beast::http::field::content_length, body_size);
        } else if ( 0 == std::strcmp(action, "PUT") ) {
            req.method(boost::beast::http::verb::put);
            auto body_size = data.size();
            req.body() = std::move(data);
            req.set(boost::beast::http::field::content_length, body_size);
        } else if ( 0 == std::strcmp(action, "DELETE") ) {
            req.method(boost::beast::http::verb::delete_);
            auto body_size = data.size();
            req.body() = std::move(data);
            req.set(boost::beast::http::field::content_length, body_size);
        } else {
            assert(!"unreachable");
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

    using ssl_socket_ptr = std::unique_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>>;
    using ssl_socket_type = typename ssl_socket_ptr::element_type;

    void async_post() {
        if ( m_write_in_process ) {
            return;
        }

        m_write_in_process = true;

        auto &front = m_async_requests.front();
        std::string action = std::move(front.action);
        std::string data = std::move(front.data);
        std::string target = front.target;
        //std::cout << "async_post(): target=" << target << std::endl;

        // Set up an HTTP GET request message
        m_req.version(11);
        if ( action == "GET" ) {
            m_req.method(boost::beast::http::verb::get);
        } else if ( action == "POST" ) {
            m_req.method(boost::beast::http::verb::post);
            auto body_size = data.size();
            m_req.body() = std::move(data);
            m_req.set(boost::beast::http::field::content_length, body_size);
        } else if ( action == "PUT" ) {
            m_req.method(boost::beast::http::verb::put);
            auto body_size = data.size();
            m_req.body() = std::move(data);
            m_req.set(boost::beast::http::field::content_length, body_size);
        } else if ( action == "DELETE" ) {
            m_req.method(boost::beast::http::verb::delete_);
        } else {
            assert(!"unreachable");
        }

        m_req.target(target);
        m_req.insert("X-MBX-APIKEY", m_pk);
        m_req.set(boost::beast::http::field::host, m_host);
        m_req.set(boost::beast::http::field::user_agent, m_client_api_string);
        m_req.set(boost::beast::http::field::content_type, "application/x-www-form-urlencoded");

        //std::cout << target << " REQUEST:\n" << m_req << std::endl;

        // Look up the domain name
        m_resolver.async_resolve(
            m_host,
            m_port,
            std::bind(
                &impl::on_resolve,
                this,
                std::placeholders::_1,
                std::placeholders::_2
            )
        );
    }
    void on_resolve(boost::system::error_code ec, boost::asio::ip::tcp::resolver::results_type results) {
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
            sptr->next_layer(),
            results.begin(),
            results.end(),
            [this, ssl_socket=std::move(ssl_socket)](boost::system::error_code ec, auto) mutable
            { on_connect(std::move(ssl_socket), ec); }
        );
    }
    void on_connect(ssl_socket_ptr ssl_socket, boost::system::error_code ec)  {
        if ( ec ) {
            m_write_in_process = false;
            process_reply(__MAKE_FILELINE, ec.value(), ec.message(), std::string{});
            return;
        }

        auto sptr = ssl_socket.get();

        // Perform the SSL handshake
        sptr->async_handshake(
            boost::asio::ssl::stream_base::client,
            [this, ssl_socket=std::move(ssl_socket)](boost::system::error_code ec) mutable
            { on_handshake(std::move(ssl_socket), ec); }
        );
    }
    void on_handshake(ssl_socket_ptr ssl_socket, boost::system::error_code ec) {
        if ( ec ) {
            m_write_in_process = false;
            process_reply(__MAKE_FILELINE, ec.value(), ec.message(), std::string{});
            return;
        }

        auto sptr = ssl_socket.get();

        // Send the HTTP request to the remote host
        boost::beast::http::async_write(
            *sptr,
            m_req,
            [this, ssl_socket=std::move(ssl_socket)](boost::system::error_code ec, std::size_t wr) mutable
            { on_write(std::move(ssl_socket), ec, wr); }
        );
    }
    void on_write(ssl_socket_ptr ssl_socket, boost::system::error_code ec, std::size_t wr) {
        boost::ignore_unused(wr);
        m_req.clear();

        if ( ec ) {
            m_write_in_process = false;
            process_reply(__MAKE_FILELINE, ec.value(), ec.message(), std::string{});
            return;
        }

        auto sptr = ssl_socket.get();

        // Receive the HTTP response
        boost::beast::http::async_read(
            *sptr,
            m_buffer,
            m_res,
            [this, ssl_socket=std::move(ssl_socket)](boost::system::error_code ec, std::size_t rd) mutable
            { on_read(std::move(ssl_socket), ec, rd); }
        );
    }
    void on_read(ssl_socket_ptr ssl_socket, boost::system::error_code ec, std::size_t rd) {
        boost::ignore_unused(rd);

        if ( ec ) {
            m_write_in_process = false;
            process_reply(__MAKE_FILELINE, ec.value(), ec.message(), std::string{});
            return;
        }

        auto sptr = ssl_socket.get();

        sptr->async_shutdown(
            [this, ssl_socket=std::move(ssl_socket)](boost::system::error_code ec) mutable
            { on_shutdown(std::move(ssl_socket), ec); }
        );
    }
    void on_shutdown(ssl_socket_ptr ssl_socket, boost::system::error_code ec) {
        (void)ssl_socket;

        // openssl issue: https://github.com/boostorg/beast/issues/38
        static const long short_read_error = 335544539;
        bool is_ssl_short_read_error =
            ec.category() == boost::asio::error::ssl_category &&
            ec.value() == short_read_error
        ;

        if ( ec == boost::asio::error::eof || is_ssl_short_read_error ) {
            ec.assign(0, ec.category());
        }

        auto body = std::move(m_res.body());
        m_res.clear();

        process_reply(__MAKE_FILELINE, 0, std::string{}, std::move(body));

        if ( m_async_requests.empty() ) {
            m_write_in_process = false;
        } else {
            m_write_in_process = false;
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
        std::string action;
        std::string data;
        detail::invoker_ptr invoker;
    };
    std::queue<async_req_item> m_async_requests;
    boost::asio::ssl::context m_ssl_ctx;
    boost::asio::ip::tcp::resolver m_resolver;
    boost::beast::flat_buffer m_buffer; // (Must persist between reads)
    boost::beast::http::request<boost::beast::http::string_body> m_req;
    boost::beast::http::response<boost::beast::http::string_body> m_res;
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
    return pimpl->post(false, "/api/v1/ping", "GET", {}, std::move(cb));
}

/*************************************************************************************************/

api::result<server_time_t> api::server_time(server_time_cb cb) {
    return pimpl->post(false, "/api/v1/time", "GET", {}, std::move(cb));
}

/*************************************************************************************************/

api::result<prices_t::price_t> api::price(const char *symbol, price_cb cb) {
    const impl::init_list_type map = {
        {"symbol", symbol}
    };

    return pimpl->post(false, "/api/v3/ticker/price", "GET", map, std::move(cb));
}

api::result<prices_t> api::prices(prices_cb cb) {
    return pimpl->post(false, "/api/v3/ticker/price", "GET", {}, std::move(cb));
}

/*************************************************************************************************/

api::result<_24hrs_tickers_t::_24hrs_ticker_t> api::_24hrs_ticker(const char *symbol, api::_24hrs_ticker_cb cb) {
    const impl::init_list_type map = {
        {"symbol", symbol}
    };

    return pimpl->post(false, "/api/v1/ticker/24hr", "GET", map, std::move(cb));
}

api::result<_24hrs_tickers_t> api::_24hrs_tickers(api::_24hrs_tickers_cb cb) {
    return pimpl->post(false, "/api/v1/ticker/24hr", "GET", {}, std::move(cb));
}

/*************************************************************************************************/

api::result<exchange_info_t> api::exchange_info(exchange_info_cb cb) {
    return pimpl->post(false, "/api/v1/exchangeInfo", "GET", {}, std::move(cb));
}

/*************************************************************************************************/

api::result<depths_t> api::depths(const char *symbol, std::size_t limit, depths_cb cb) {
    const impl::init_list_type map = {
         {"symbol", symbol}
        ,{"limit", limit}
    };

    return pimpl->post(false, "/api/v1/depth", "GET", map, std::move(cb));
}

/*************************************************************************************************/

api::result<trades_t::trade_t> api::trade(const char *symbol, trade_cb cb) {
    const impl::init_list_type map = {
         {"symbol", symbol}
        ,{"limit", 1u}
    };

    return pimpl->post(false, "/api/v1/trades", "GET", map, std::move(cb));
}

api::result<trades_t> api::trades(const char *symbol, std::size_t limit, trades_cb cb) {
    const impl::init_list_type map = {
         {"symbol", symbol}
        ,{"limit", limit}
    };

    return pimpl->post(false, "/api/v1/trades", "GET", map, std::move(cb));
}

/*************************************************************************************************/

api::result<agg_trades_t::agg_trade_t> api::agg_trade(const char *symbol, agg_trade_cb cb) {
    const impl::init_list_type map = {
         {"symbol", symbol}
        ,{"limit", 1u}
    };

    return pimpl->post(false, "/api/v1/aggTrades", "GET", map, std::move(cb));
}

api::result<agg_trades_t> api::agg_trades(const char *symbol, std::size_t limit, agg_trades_cb cb) {
    const impl::init_list_type map = {
         {"symbol", symbol}
        ,{"limit", limit}
    };

    return pimpl->post(false, "/api/v1/aggTrades", "GET", map, std::move(cb));
}

/*************************************************************************************************/

api::result<klines_t> api::klines(const char *symbol, const char *interval, std::size_t limit, klines_cb cb) {
    const impl::init_list_type map = {
         {"symbol", symbol}
        ,{"limit", limit}
        ,{"interval", interval}
    };

    return pimpl->post(false, "/api/v1/klines", "GET", map, std::move(cb));
}

/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/

api::result<account_info_t> api::account_info(account_info_cb cb) {
    return pimpl->post(true, "/api/v3/account", "GET", {}, std::move(cb));
}

/*************************************************************************************************/

api::result<order_info_t> api::order_info(const char *symbol, std::size_t orderid, const char *client_orderid, order_info_cb cb) {
    const impl::init_list_type map = {
         {"symbol", symbol}
        ,{"orderId", orderid}
        ,{"origClientOrderId", client_orderid}
    };

    return pimpl->post(true, "/api/v3/order", "GET", map, std::move(cb));
}

/*************************************************************************************************/

api::result<orders_info_t> api::open_orders(const char *symbol, open_orders_cb cb) {
    const impl::init_list_type map = {
        {"symbol", symbol}
    };

    return pimpl->post(true, "/api/v3/openOrders", "GET", map, std::move(cb));
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

    return pimpl->post(true, "/api/v3/allOrders", "GET", map, std::move(cb));
}

/*************************************************************************************************/

const char* order_type_to_string(e_type type) {
    return type == e_type::limit ? "LIMIT"
        : type == e_type::market ? "MARKET"
            : type == e_type::stop_loss ? "STOP_LOSS"
                : type == e_type::stop_loss_limit ? "STOP_LOSS_LIMIT"
                    : type == e_type::take_profit ? "TAKE_PROFIT"
                        : type == e_type::take_profit_limit ? "TAKE_PROFIT_LIMIT"
                            : type == e_type::limit_maker ? "LIMIT_MAKER"
                                : nullptr
    ;
}

const char* order_time_to_string(e_time time) {
    return time == e_time::GTC ? "GTC"
        : time == e_time::FOK ? "FOK"
            : time == e_time::IOC ? "IOC"
                : nullptr
    ;
}

const char* order_side_to_string(e_side side) {
    return side == e_side::buy ? "BUY"
        : side == e_side::sell ? "SELL"
            : nullptr
    ;
}

api::result<new_order_resp_type>
api::new_order(
     const char *symbol
    ,const e_side side
    ,const e_type type
    ,const e_time time
    ,const char *amount
    ,const char *price
    ,const char *client_order_id
    ,const char *stop_price
    ,const char *iceberg_amount
    ,new_order_cb cb
) {
    const char *side_str = order_side_to_string(side);
    assert(side_str);

    const char *type_str = order_type_to_string(type);
    assert(type_str);

    const char *time_str = type == e_type::market
        ? nullptr
        : order_time_to_string(time)
    ;

    // TODO
    const char *responce_type = 1 /*type == e_type::market*/
        ? "FULL"
        : "RESULT"
    ;

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
        ,{"newOrderRespType", responce_type}
    };

    return pimpl->post(true, "/api/v3/order", "POST", map, std::move(cb));
}

/*************************************************************************************************/

api::result<new_order_resp_type> api::new_test_order(
     const char *symbol
    ,const e_side side
    ,const e_type type
    ,const e_time time
    ,const char *amount
    ,const char *price
    ,const char *client_order_id
    ,const char *stop_price
    ,const char *iceberg_amount
    ,new_order_cb cb
) {
    const char *side_str = order_side_to_string(side);
    assert(side_str);

    const char *type_str = order_type_to_string(type);
    assert(type_str);

    const char *time_str = type == e_type::market ? nullptr : order_time_to_string(time);

    static const char *responce_type = "RESULT";

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
        ,{"newOrderRespType", responce_type}
    };

    return pimpl->post(true, "/api/v3/order/test", "POST", map, std::move(cb));
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

    return pimpl->post(true, "/api/v3/order", "DELETE", map, std::move(cb));
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

    return pimpl->post(true, "/api/v3/myTrades", "GET", map, std::move(cb));
}

/*************************************************************************************************/

api::result<start_user_data_stream_t> api::start_user_data_stream(start_user_data_stream_cb cb) {
    return pimpl->post(false, "/api/v1/userDataStream", "POST", {}, std::move(cb));
}

/*************************************************************************************************/

api::result<ping_user_data_stream_t> api::ping_user_data_stream(const char *listen_key, ping_user_data_stream_cb cb) {
    const impl::init_list_type map = {
        {"listenKey", listen_key}
    };

    return pimpl->post(false, "/api/v1/userDataStream", "PUT", map, std::move(cb));
}

/*************************************************************************************************/

api::result<close_user_data_stream_t> api::close_user_data_stream(const char *listen_key, close_user_data_stream_cb cb) {
    const impl::init_list_type map = {
        {"listenKey", listen_key}
    };

    return pimpl->post(false, "/api/v1/userDataStream", "DELETE", map, std::move(cb));
}

/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/

} // ns rest
} // ns binapi
