
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

int main() {
    boost::asio::io_context ioctx;

    binapi::ws::websockets_pool ws{
         ioctx
        ,"stream.binance.com"
        ,"9443"
    };

    ws.depth("BTCUSDT",
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

    ws.book("BTCUSDT",
        [](const char *fl, int ec, std::string emsg, auto book) {
            if ( ec ) {
                std::cerr << "subscribe book error: fl=" << fl << ", ec=" << ec << ", emsg=" << emsg << std::endl;

                return false;
            }

            std::cout << "book: " << book << std::endl;

            return true;
        }
    );

    ws.books(
        [](const char *fl, int ec, std::string emsg, auto books) {
            if ( ec ) {
                std::cerr << "subscribe books error: fl=" << fl << ", ec=" << ec << ", emsg=" << emsg << std::endl;

                return false;
            }

            std::cout << "books: " << books << std::endl;

            return true;
        }
    );

    ioctx.run();

    return EXIT_SUCCESS;
}
