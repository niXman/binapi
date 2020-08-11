
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

    auto res = api.price("BTCUSDT");
    if ( !res ) {
        std::cerr << "get price error: " << res.errmsg << std::endl;

        return EXIT_FAILURE;
    }

    std::cout << "price: " << res.v << std::endl;

    return EXIT_SUCCESS;
}
