
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

    api.price("BTCUSDT",
        [](const char *fl, int ec, std::string emsg, auto res) {
            if ( ec ) {
                std::cerr << "get price error: fl=" << fl << ", ec=" << ec << ", emsg=" << emsg << std::endl;

                return false;
            }

            std::cout << "price: " << res << std::endl;

            return true;
        }
    );

    ioctx.run();

    return EXIT_SUCCESS;
}
