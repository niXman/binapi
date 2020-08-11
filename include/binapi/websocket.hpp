
// ----------------------------------------------------------------------------
//                              Apache License
//                        Version 2.0, January 2004
//                     http://www.apache.org/licenses/
//
// This file is part of binapi(https://github.com/niXman/binapi) project.
//
// Copyright (c) 2019-2020 niXman (github dot nixman dog pm.me). All rights reserved.
// ----------------------------------------------------------------------------

#ifndef __binapi__websocket_hpp
#define __binapi__websocket_hpp

#include <memory>
#include <functional>

#include <boost/asio/io_context.hpp>

namespace binapi {
namespace userdata {

struct account_update_t;
struct order_update_t;

} // ns userdata

namespace ws {

struct depths_t;
struct trade_t;
struct agg_trade_t;
struct kline_t;
struct market_ticker_t;
struct markets_tickers_t;

/*************************************************************************************************/

struct websocket {
    websocket(const websocket &) = delete;
    websocket& operator= (const websocket &) = delete;
    websocket(websocket &&) noexcept = default;
    websocket& operator= (websocket &&) noexcept = default;

    explicit websocket(boost::asio::io_context &ioctx);
    ~websocket();

    using on_message_received_cb = std::function<
        bool(const char *fl, int ec, std::string errmsg, const char *ptr, std::size_t size)
    >; // when 'false' returned the stop will called
    void start(const std::string &host, const std::string &port, const std::string &target, on_message_received_cb cb);
    void stop();

private:
    struct impl;
    std::unique_ptr<impl> pimpl;
};

/*************************************************************************************************/

struct websockets_pool {
    websockets_pool(const websockets_pool &) = delete;
    websockets_pool& operator= (const websockets_pool &) = delete;
    websockets_pool(websockets_pool &&) noexcept = default;
    websockets_pool& operator= (websockets_pool &&) noexcept = default;

    using on_message_received_cb = std::function<void(const char *channel, const char *ptr, std::size_t size)>;

    websockets_pool(
         boost::asio::io_context &ioctx
        ,std::string host
        ,std::string port
        ,on_message_received_cb cb = {}
    );
    ~websockets_pool();

    using handle = void *;

    using on_depth_received_cd = std::function<bool(const char *fl, int ec, std::string errmsg, depths_t msg)>;
    handle subscribe_depth(const char *pair, on_depth_received_cd cb);
    void unsubscribe_depth(handle h);

    using on_kline_received_cd = std::function<bool(const char *fl, int ec, std::string errmsg, kline_t msg)>;
    // period - 1m, 3m, 5m, 15m, 30m, 1h, 2h, 4h, 6h, 8h, 12h, 1d, 3d, 1w, 1M
    handle subscribe_klines(const char *pair, const char *period, on_kline_received_cd cb);
    void unsubscribe_klines(handle h);

    using on_trade_received_cd = std::function<bool(const char *fl, int ec, std::string errmsg, trade_t msg)>;
    handle subscribe_trade(const char *pair, on_trade_received_cd cb);
    void unsubscribe_trade(handle h);

    using on_agg_trade_received_cd = std::function<bool(const char *fl, int ec, std::string errmsg, agg_trade_t msg)>;
    handle subscribe_agg_trade(const char *pair, on_agg_trade_received_cd cb);
    void unsubscribe_agg_trade(handle h);

    using on_market_received_cd = std::function<bool(const char *fl, int ec, std::string errmsg, market_ticker_t msg)>;
    handle subscribe_market(const char *pair, on_market_received_cd cb);
    void unsubscribe_market(handle h);

    using on_markets_received_cd = std::function<bool(const char *fl, int ec, std::string errmsg, markets_tickers_t msg)>;
    handle subscribe_markets(on_markets_received_cd cb);
    void unsubscribe_markets(handle h);

    using on_order_update_cb = std::function<bool(const char *fl, int ec, std::string errmsg, userdata::order_update_t msg)>;
    using on_account_update_cb = std::function<bool(const char *fl, int ec, std::string errmsg, userdata::account_update_t msg)>;
    handle subscribe_userdata(const char *lkey, on_order_update_cb ocb, on_account_update_cb acb);
    void unsubscribe_userdata(handle h);

    void unsubscribe_all();

private:
    struct impl;
    std::unique_ptr<impl> pimpl;
};

/*************************************************************************************************/

} // ns ws
} // ns binapi

#endif // __binapi__websocket_hpp
