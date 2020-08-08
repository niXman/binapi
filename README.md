# binapi
Binance API implemented in C++ for both synchronous and asynchronous way.

# Motivation
This implementation has been developed as a consequence of the lack of suitable alternatives as part of my multiuser trading platform project.

# REST API
- ping
- time
- exchange info
- account info
- depth
- trades
- agg trades
- 24hr ticker
- price
- order put
- order query
- order test put
- order test query
- order cancel
- open orders query
- all orders query
- my trades
- user-data-stream new
- user-data-stream ping
- user-data-stream close

# WebSocket API
- depth
- klines
- trades
- agg trades
- ticker
- all market tickers
- userdata

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
        ,10000
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
        ,10000
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
