
// ----------------------------------------------------------------------------
//                              Apache License
//                        Version 2.0, January 2004
//                     http://www.apache.org/licenses/
//
// This file is part of bg_api(https://github.com/patrickk33/bg_api) project. A fork of 
// niXman's binapi(https://github.com/niXman/binapi) project.
//
// Copyright (c) 2019-2021 niXman (github dot nixman dog pm.me). All rights reserved.
// ----------------------------------------------------------------------------

#include <SDKDDKVer.h> // defines the highest available Windows platform

#include "bg_api/websocket.hpp"
#include "bg_api/api.hpp"
#include "bg_api/flatjson.hpp"
#include "bg_api/simdjson.h"

#include <boost/asio/io_context.hpp>
#include <boost/asio/steady_timer.hpp>

#include <iostream>
#include <fstream>
#include <bg_api/errors.hpp>

/*************************************************************************************************/

std::string read_file(const char *fname) {
    std::ifstream file(fname);
    assert(file);

    std::string res{
        (std::istreambuf_iterator<char>(file)),
        (std::istreambuf_iterator<char>())
    };

    return res;
}

/*************************************************************************************************/

#define PRINT_IF_ERROR(res) \
    if ( !static_cast<bool>(res) ) { \
        std::cout << __FILE__ << "(" << __LINE__ << "): msg=" << res.errmsg << std::endl; \
    }

/*************************************************************************************************/

#define BREAK_IF_ERROR(res) \
    PRINT_IF_ERROR(res); \
    if ( !res ) { \
        return EXIT_FAILURE; \
    }

/*************************************************************************************************/

int main(int argc, char **argv) {
    //assert(argc == 3);
    //const std::string pk = argv[1];
    //const std::string passphrase = argv[2];
    //const std::string sk = argv[3];

    std::cout.precision(15);

    boost::asio::io_context ioctx;
    //bg_api::ws::websockets wsp(ioctx, "ws.bitget.com", "443", pk, passphrase, sk);

    bg_api::rest::api api(
        ioctx,
        "api.bitget.com",
        "443",
        "", // apikey
        "", // passphrase
        "", // secret
        30
    );    
    
    api.getServerTime(
        [](const char *fl, int ec, std::string errmsg, bg_api::rest::server_time_t res) {
            
            if (ec) {
                std::cerr << "get time error: fl=" << fl << ", ec=" << ec << ", errmsg=" << errmsg << std::endl;
                return false;
            }

            //std::cout << "server time: " << res.serverTime << std::endl;
            return true;
        }
    );
    
    api.getCurrencies(
        [](const char *fl, int ec, std::string errmsg, bg_api::rest::currencies_t res) {
            if (ec) {
                std::cerr << "get coin list error: fl=" << fl << ", ec=" << ec << ", errmsg=" << errmsg << std::endl;
                return false;
            }

            //std::cout << res << std::endl;
            
            return true;
        }
    );

    api.getProduct("BTCUSDT_SPBL",
        [](const char *fl, int ec, std::string errmsg, bg_api::rest::product_t res) {
            if (ec) {
                std::cerr << "get product error: fl=" << fl << ", ec=" << ec << ", errmsg=" << errmsg << std::endl;
                return false;
            }

            //std::cout << res << std::endl;
            
            return true;
        }
    );

    api.getProducts(
        [](const char *fl, int ec, std::string errmsg, bg_api::rest::products_t res) {
            if (ec) {
                std::cerr << "get products error: fl=" << fl << ", ec=" << ec << ", errmsg=" << errmsg << std::endl;
                return false;
            }

            //std::cout << res << std::endl;
            
            return true;
        }
    );

    api.getSpotTicker("BTCUSDT_SPBL",
        [](const char *fl, int ec, std::string errmsg, bg_api::rest::spot_ticker_t res) {
            if (ec) {
                std::cerr << "get ticker error: fl=" << fl << ", ec=" << ec << ", errmsg=" << errmsg << std::endl;
                return false;
            }

            //std::cout << res << std::endl;
            
            return true;
        }
    );

    api.getSpotTickers(
        [](const char *fl, int ec, std::string errmsg, bg_api::rest::spot_tickers_t res) {
            if (ec) {
                std::cerr << "get tickers error: fl=" << fl << ", ec=" << ec << ", errmsg=" << errmsg << std::endl;
                return false;
            }

            //std::cout << res << std::endl;
            
            return true;
        }
    );

    api.getSpotFills("BTCUSDT_SPBL",
        [](const char *fl, int ec, std::string errmsg, bg_api::rest::spot_fills_t res) {
            if (ec) {
                std::cerr << "get fills error: fl=" << fl << ", ec=" << ec << ", errmsg=" << errmsg << std::endl;
                return false;
            }

            std::cout << res << std::endl;
            
            return true;
        }
    );

    ioctx.run();

    return 0;
}

/*************************************************************************************************/
