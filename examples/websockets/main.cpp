
// ----------------------------------------------------------------------------
//                              Apache License
//                        Version 2.0, January 2004
//                     http://www.apache.org/licenses/
//
// This file is part of binapi(https://github.com/niXman/binapi) project.
//
// Copyright (c) 2019-2021 niXman (github dot nixman dog pm.me). All rights reserved.
// ----------------------------------------------------------------------------

#include <iostream>

#include <boost/asio/io_context.hpp>
#include <boost/asio/steady_timer.hpp>

#include <binapi/api.hpp>
#include <binapi/websocket.hpp>

int main() {
    boost::asio::io_context ioctx;

    binapi::ws::websockets ws{
         ioctx
        ,"stream.binance.com"
        ,"9443"
    };

    ws.klines("BTCUSDT", "1s",
        [](const char *fl, int ec, std::string emsg, auto klines) {
            if ( ec ) {
                std::cerr << "subscribe klines error: fl=" << fl << ", ec=" << ec << ", emsg=" << emsg << std::endl;

                return false;
            }

            std::cout << "klines: " << klines << std::endl;

            return true;
        }
    );

    ws.part_depth("BTCUSDT", binapi::e_levels::_5, binapi::e_freq::_100ms,
        [](const char *fl, int ec, std::string emsg, auto depths) {
            if ( ec ) {
                std::cerr << "subscribe part_depth error: fl=" << fl << ", ec=" << ec << ", emsg=" << emsg << std::endl;

                return false;
            }

            std::cout << "part_depths: " << depths << std::endl;

            return true;
          }
    );

    ws.diff_depth("BTCUSDT", binapi::e_freq::_100ms,
        [](const char *fl, int ec, std::string emsg, auto depths) {
            if ( ec ) {
                std::cerr << "subscribe diff_depth error: fl=" << fl << ", ec=" << ec << ", emsg=" << emsg << std::endl;

                return false;
            }

            std::cout << "diff_depths: " << depths << std::endl;

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

    auto book_handler = ws.book("BTCUSDT",
        [](const char *fl, int ec, std::string emsg, auto book) {
            if ( ec ) {
                std::cerr << "subscribe book error: fl=" << fl << ", ec=" << ec << ", emsg=" << emsg << std::endl;

                return false;
            }

            std::cout << "book: " << book << std::endl;

            return true;
        }
    );

    ws.mini_tickers(
        [](const char *fl, int ec, std::string emsg, auto mini_tickers) {
            if ( ec ) {
                std::cerr << "subscribe mini_tickers error: fl=" << fl << ", ec=" << ec << ", emsg=" << emsg << std::endl;

                return false;
            }

            std::cout << "mini_tickers: " << mini_tickers << std::endl;

            return true;
        }
    );

#if 1
    boost::asio::steady_timer timer0{ioctx, std::chrono::steady_clock::now() + std::chrono::seconds(5)};
    timer0.async_wait([&ws, book_handler](const auto &/*ec*/){
        std::cout << "unsubscribing book_handler: " << book_handler << std::endl;
        ws.unsubscribe(book_handler);
    });
#endif

    boost::asio::steady_timer timer2{ioctx, std::chrono::steady_clock::now() + std::chrono::seconds(300)};
    timer2.async_wait([&ws](const auto &/*ec*/){
        std::cout << "async unsubscribing all" << std::endl;
        ws.async_unsubscribe_all();
    });

    ioctx.run();

    return EXIT_SUCCESS;
}
