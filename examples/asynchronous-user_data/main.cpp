
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
#include <binapi/websocket.hpp>

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
    binapi::ws::websockets ws(
         ioctx
        ,"stream.binance.com"
        ,"9443"
    );

    auto res = api.account_info(
        [](const char *fl, int ec, std::string emsg, auto res) {
            if ( ec ) {
                std::cerr << "get account info error: fl=" << fl << ", ec=" << ec << ", emsg=" << emsg << std::endl;

                return false;
            }

            std::cout << "account info: " << res << std::endl;

            return true;
        }
    );

    auto start_uds = api.start_user_data_stream();
    assert(start_uds);
    std::cout << "start_uds=" << start_uds.v << std::endl << std::endl;

    ws.userdata(start_uds.v.listenKey.c_str(),
        [](const char *fl, int ec, std::string errmsg, binapi::userdata::account_update_t msg) -> bool {
            if ( ec ) {
                std::cout << "account update: fl=" << fl << ", ec=" << ec << ", errmsg: " << errmsg << ", msg: " << msg << std::endl;
                return false;
            }

            std::cout << "account update:\n" << msg << std::endl;
            return true;
        }
        ,[](const char *fl, int ec, std::string errmsg, binapi::userdata::balance_update_t msg) -> bool {
            if ( ec ) {
                std::cout << "balance update: fl=" << fl << ", ec=" << ec << ", errmsg: " << errmsg << ", msg: " << msg << std::endl;
                return false;
            }

            std::cout << "balance update:\n" << msg << std::endl;
            return true;
        }
        ,[](const char *fl, int ec, std::string errmsg, binapi::userdata::order_update_t msg) -> bool {
            if ( ec ) {
                std::cout << "order update: fl=" << fl << ", ec=" << ec << ", errmsg: " << errmsg << ", msg: " << msg << std::endl;
                return false;
            }

            std::cout << "order update:\n" << msg << std::endl;
            return true;
        }
    );

    while ( true ) {
        try {
            ioctx.run();
            break;
        } catch (const std::exception &ex) {
            std::cerr << "std::exception: what: " << ex.what() << std::endl;

            ioctx.restart();
        }
    }

    return EXIT_SUCCESS;
}
