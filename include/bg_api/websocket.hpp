
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

#ifndef __bg_api__websocket_hpp
#define __bg_api__websocket_hpp

#include "enums.hpp"

#include <memory>
#include <functional>

namespace boost {
namespace asio {
class io_context;
} // ns asio
} // ns boost

namespace bg_api {
namespace userdata {

struct account_update_t;
struct balance_update_t;
struct order_update_t;

} // ns userdata

namespace ws {

struct part_depths_t;
struct diff_depths_t;
struct trade_t;
struct agg_trade_t;
struct kline_t;
struct mini_ticker_t;
struct mini_tickers_t;
struct market_ticker_t;
struct markets_tickers_t;
struct book_ticker_t;

/*************************************************************************************************/

struct websockets {
    websockets(const websockets &) = delete;
    websockets& operator= (const websockets &) = delete;
    websockets(websockets &&) noexcept = default;
    websockets& operator= (websockets &&) noexcept = default;

    using on_message_received_cb = std::function<void(const char *channel, const char *ptr, std::size_t size)>;

    websockets(
        boost::asio::io_context &ioctx,
        std::string host,
        std::string port,
        std::string pk,
        std::string passphrase,
        std::string sk,
        on_message_received_cb cb = {}
    );
    ~websockets();

    using handle = void *;

    

    void unsubscribe(const handle &h);
    void async_unsubscribe(const handle &h);
    void unsubscribe_all();
    void async_unsubscribe_all();

private:
    struct impl;
    std::unique_ptr<impl> pimpl;
};

/*************************************************************************************************/

} // ns ws
} // ns bg_api

#endif // __bg_api__websocket_hpp
