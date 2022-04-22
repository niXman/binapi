
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

#include <boost/asio/io_context.hpp>

#include <iostream>

int main() {
    boost::asio::io_context ioctx;
    binapi::rest::api api{
         ioctx
        ,"api.binance.com"
        ,"443"
        ,"" // can be empty for non USER_DATA reqs
        ,"" // can be empty for non USER_DATA reqs
        ,10000 // recvWindow
    };

    auto res0 = api.exchange_info("BTCUSDT");
    if ( !res0 ) {
        std::cerr << "exchange_info error: " << res0.errmsg << std::endl;

        return EXIT_FAILURE;
    }
    std::cout << "exchange info: " << res0.v << std::endl << std::endl;

    auto res1 = api.price("BTCUSDT");
    if ( !res1 ) {
        std::cerr << "get price error: " << res1.errmsg << std::endl;

        return EXIT_FAILURE;
    }
    std::cout << "price: " << res1.v << std::endl << std::endl;

    auto res2 = api.avg_price("BTCUSDT");
    if ( !res2 ) {
        std::cerr << "get avg price error: " << res2.errmsg << std::endl;

        return EXIT_FAILURE;
    }
    std::cout << "avg price: " << res2.v << std::endl << std::endl;

    return EXIT_SUCCESS;
}
