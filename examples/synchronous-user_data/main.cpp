
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

int main(int argc, char **argv) {
    assert(argc == 3);
    std::string pk = argv[1];
    std::string sk = argv[2];

    boost::asio::io_context ioctx;

    binapi::rest::api api{
        ioctx
        ,"api.binance.com"
        ,"443"
        ,std::move(pk)
        ,std::move(sk)
        ,10000 // recvWindow
    };

    auto res = api.account_info();
    if ( !res ) {
        std::cerr << "get account info error: " << res.errmsg << std::endl;

        return EXIT_FAILURE;
    }

    std::cout << "account info: " << res.v << std::endl;

    return EXIT_SUCCESS;
}
