# binapi
Binance API implemented in C++ for both synchronous and asynchronous way.

# Donate
BTC: 3BJKvx6LyKB2J5KgRBqst415KKmwQE5eQX

# Motivation
This implementation has been developed as a consequence of the lack of suitable alternatives as part of my multiuser trading platform project.

# REST API
- [Test connectivity](https://github.com/binance/binance-spot-api-docs/blob/master/rest-api.md#test-connectivity) -> `api::ping()`
- [Check server time](https://github.com/binance/binance-spot-api-docs/blob/master/rest-api.md#check-server-time) -> `api::time()`
- [Exchange information](https://github.com/binance/binance-spot-api-docs/blob/master/rest-api.md#exchange-information) -> `api::exchange_info()`
- [Account information](https://github.com/binance/binance-spot-api-docs/blob/master/rest-api.md#account-information-user_data) -> `api::account_info()`
- [Order book](https://github.com/binance/binance-spot-api-docs/blob/master/rest-api.md#order-book) -> `api::depth()`
- [Recent trades list](https://github.com/binance/binance-spot-api-docs/blob/master/rest-api.md#recent-trades-list) -> `api::trades()`
- [Aggregate trades list](https://github.com/binance/binance-spot-api-docs/blob/master/rest-api.md#compressedaggregate-trades-list) -> `api::agg_trades()`
- [24hr ticker price change statistics](https://github.com/binance/binance-spot-api-docs/blob/master/rest-api.md#24hr-ticker-price-change-statistics) -> `api::_24hrs_ticker()`
- [Symbol price ticker](https://github.com/binance/binance-spot-api-docs/blob/master/rest-api.md#symbol-price-ticker) -> `api::price()`
- [New order](https://github.com/binance/binance-spot-api-docs/blob/master/rest-api.md#new-order--trade) -> `api::new_order()`
- [Query order](https://github.com/binance/binance-spot-api-docs/blob/master/rest-api.md#query-order-user_data) -> `api::order_info()`
- [Test new order](https://github.com/binance/binance-spot-api-docs/blob/master/rest-api.md#test-new-order-trade) -> `api::new_test_order()`
- [Cancel order](https://github.com/binance/binance-spot-api-docs/blob/master/rest-api.md#cancel-order-trade) -> `api::cancel_order()`
- [Current open orders](https://github.com/binance/binance-spot-api-docs/blob/master/rest-api.md#current-open-orders-user_data) -> `api::open_orders()`
- [All orders](https://github.com/binance/binance-spot-api-docs/blob/master/rest-api.md#all-orders-user_data) -> `api::all_orders()`
- [Account trade list](https://github.com/binance/binance-spot-api-docs/blob/master/rest-api.md#account-trade-list-user_data) -> `api::my_trades()`
- [Start user data stream](https://github.com/binance/binance-spot-api-docs/blob/master/rest-api.md#start-user-data-stream-user_stream) -> `api::start_user_data_stream()`
- [Keepalive user data stream](https://github.com/binance/binance-spot-api-docs/blob/master/rest-api.md#keepalive-user-data-stream-user_stream) -> `api::ping_user_data_stream()`
- [Close user data stream](https://github.com/binance/binance-spot-api-docs/blob/master/rest-api.md#close-user-data-stream-user_stream) -> `api::close_user_data_stream()`

# WebSocket API
- [Partial Book Depth Streams](https://github.com/binance/binance-spot-api-docs/blob/master/web-socket-streams.md#partial-book-depth-streams) -> `websockets::part_depth()`
- [Diff. Depth Stream](https://github.com/binance/binance-spot-api-docs/blob/master/web-socket-streams.md#diff-depth-stream) -> `websockets::diff_depth()`
- [Kline/Candlestick Streams](https://github.com/binance/binance-spot-api-docs/blob/master/web-socket-streams.md#klinecandlestick-streams) -> `websockets::klines()`
- [Trade Streams](https://github.com/binance/binance-spot-api-docs/blob/master/web-socket-streams.md#trade-streams) -> `websockets::trade()`
- [Aggregate Trade Streams](https://github.com/binance/binance-spot-api-docs/blob/master/web-socket-streams.md#aggregate-trade-streams) -> `websockets::agg_trade()`
- [Individual Symbol Ticker Streams](https://github.com/binance/binance-spot-api-docs/blob/master/web-socket-streams.md#individual-symbol-ticker-streams) -> `websockets::market()`
- [All Market Tickers Stream](https://github.com/binance/binance-spot-api-docs/blob/master/web-socket-streams.md#all-market-tickers-stream) -> `websockets::markets()`
- [Individual Symbol Book Ticker Streams](https://github.com/binance/binance-spot-api-docs/blob/master/web-socket-streams.md#individual-symbol-book-ticker-streams) -> `websockets::book()`
- [All Book Tickers Stream](https://github.com/binance/binance-spot-api-docs/blob/master/web-socket-streams.md#all-book-tickers-stream) -> `websockets::books()`
- [User Data Streams](https://github.com/binance-exchange/binance-official-api-docs/blob/master/user-data-stream.md) -> `websockets::userdata()`

# Implementation details
The project is written using C++14 and [boost](https://www.boost.org/) (at least version 1.70). [boost.beast](https://www.boost.org/doc/libs/1_73_0/libs/beast/index.html) is used to interact with the network.

# Synchronous example
```cpp
#include "binapi/api.hpp"

#include <boost/asio/io_context.hpp>

#include <iostream>

int main() {
    const std::string pk = "...";
    const std::string sk = "...";

    boost::asio::io_context ioctx;
    binapi::rest::api api(
        ioctx
        ,"api.binance.com"
        ,"443"
        ,pk
        ,sk
        ,10000 // recvWindow
    );

    auto account = api.account_info();
    if ( !account ) {
        std::cerr << "account info error: " << account.errmsg << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "account info: " << account.v << std::endl << std::endl;

    return EXIT_SUCCESS;
}

```

# Asynchronous example
```cpp
#include "binapi/api.hpp"

#include <boost/asio/io_context.hpp>

#include <iostream>

int main() {
    const std::string pk = "...";
    const std::string sk = "...";

    boost::asio::io_context ioctx;
    binapi::rest::api api(
        ioctx
        ,"api.binance.com"
        ,"443"
        ,pk
        ,sk
        ,10000 // recvWindow
    );

    api.account_info([](const char *fl, int ec, std::string errmsg, binapi::rest::account_info_t res) {
        if ( ec ) {
            std::cerr << "account info error: fl=" << fl << ", ec=" << ec << ", emsg=" << errmsg << std::endl;
            return false;
        }

        std::cout << "account info: " << res << std::endl;

        return true;
    });

    ioctx.run();

    return EXIT_SUCCESS;
}
```

# WebSocket example
```cpp
#include <binapi/api.hpp>
#include <binapi/websocket.hpp>

#include <boost/asio/io_context.hpp>

#include <iostream>

int main() {
    boost::asio::io_context ioctx;

    binapi::ws::websockets ws{
         ioctx
        ,"stream.binance.com"
        ,"9443"
    };

    ws.part_depth("BTCUSDT",
        [](const char *fl, int ec, std::string emsg, auto depths) {
            if ( ec ) {
                std::cerr << "subscribe depth error: fl=" << fl << ", ec=" << ec << ", emsg=" << emsg << std::endl;

                return false;
            }

            std::cout << "depths: " << depths << std::endl;

            return true;
        }
    );

    ws.trade("BTCUSDT",
        [](const char *fl, int ec, std::string emsg, auto trades) {
            if ( ec ) {
                std::cerr << "subscribe trades error: fl=" << fl << ", ec=" << ec << ", emsg=" << emsg << std::endl;

                return false;
            }

            std::cout << "trades: " << trades << std::endl;

            return true;
        }
    );

    ioctx.run();

    return EXIT_SUCCESS;
}
```

# Tools (will write later...)
- filters
- report generators
