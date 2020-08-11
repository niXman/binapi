
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
#include <binapi/websocket.hpp>

#include <boost/asio/io_context.hpp>

#include <iostream>

int main(int argc, char **argv) {
    assert(argc == 3);

    boost::asio::io_context ioctx;
    std::string pk = argv[1];
    std::string sk = argv[2];

    binapi::rest::api api{
        ioctx
        ,"api.binance.com"
        ,"443"
        ,std::move(pk)
        ,std::move(sk)
        ,10000 // recvWindow
    };

    binapi::ws::websockets_pool ws{
         ioctx
        ,"stream.binance.com"
        ,"9443"
    };

    auto res = api.start_user_data_stream();
    if ( !res ) {
        std::cerr << "start user data stream error: " << res.errmsg << std::endl;

        return EXIT_FAILURE;
    }

    std::cout << "listen key: " << res.v << std::endl;

    ws.subscribe_depth("BTCUSDT",
        [](const char *fl, int ec, std::string emsg, auto depths) {
            if ( ec ) {
                std::cerr << "subscribe depth error: fl=" << fl << ", ec=" << ec << ", emsg=" << emsg << std::endl;

                return false;
            }

            std::cout << "depths: " << depths << std::endl;

            return true;
        }
    );

    ws.subscribe_trade("BTCUSDT",
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
