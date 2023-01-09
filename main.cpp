
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
        "",
        "",
        "",
        30
    );    
    
    int64_t currTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    std::cout << "currTime: " << currTime << std::endl;
    api.getServerTime(
        [&currTime](const char *fl, int ec, std::string errmsg, bg_api::rest::server_time_t res) {
            
            if (ec) {
                std::cerr << "get time error: fl=" << fl << ", ec=" << ec << ", errmsg=" << errmsg << std::endl;
                return false;
            }

            int64_t currTime2 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            std::cout << "ping: " << currTime2 - currTime << std::endl;
            std::cout << "currTime2: " << currTime2 << std::endl;
            currTime = currTime2;
            return true;
        }
    );
    
    api.getCoinList(
        [](const char *fl, int ec, std::string errmsg, bg_api::rest::coin_list_t res) {
            if (ec) {
                std::cerr << "get coin list error: fl=" << fl << ", ec=" << ec << ", errmsg=" << errmsg << std::endl;
                return false;
            }

            std::cout << "coin list" << std::endl;
            
            return true;
        }
    );
    
    api.getSymbols(
        [](const char *fl, int ec, std::string errmsg, bg_api::rest::symbols_t res) {
            if (ec) {
                std::cerr << "get symbols error: fl=" << fl << ", ec=" << ec << ", errmsg=" << errmsg << std::endl;
                return false;
            }

            std::cout << "symbols" << std::endl;

            return true;
        }
    );

    api.getSymbol("BTCUSDT",
        [](const char *fl, int ec, std::string errmsg, bg_api::rest::symbol_t res) {
            if (ec) {
                std::cerr << "get symbol error: fl=" << fl << ", ec=" << ec << ", errmsg=" << errmsg << std::endl;
                return false;
            }

            std::cout << "symbol BTCUSDT" << std::endl;
            
            return true;
        }
    );

    api.getSymbol("BTCUSDT_SPBL",
        [](const char *fl, int ec, std::string errmsg, bg_api::rest::symbol_t res) {
            if (ec) {
                std::cerr << "get symbol error: fl=" << fl << ", ec=" << ec << ", errmsg=" << errmsg << std::endl;
                return false;
            }

            std::cout << "symbol BTCUSDT_SPBL" << std::endl;

            return true;
        }
    );
    
    api.getSpotTicker("BTCUSDT",
        [](const char *fl, int ec, std::string errmsg, bg_api::rest::spot_ticker_t res) {
            if (ec) {
                std::cerr << "get spot ticker error: fl=" << fl << ", ec=" << ec << ", errmsg=" << errmsg << std::endl;
                return false;
            }

            std::cout << "spot ticker BTCUSDT" << std::endl;

            return true;
        }
    );

    api.getSpotTicker("BTCUSDT_SPBL",
        [](const char *fl, int ec, std::string errmsg, bg_api::rest::spot_ticker_t res) {
            if (ec) {
                std::cerr << "get spot ticker error: fl=" << fl << ", ec=" << ec << ", errmsg=" << errmsg << std::endl;
                return false;
            }

            std::cout << "spot ticker BTCUSDT_SPBL" << std::endl;

            return true;
        }
    );

    api.getSpotTickers(
        [](const char *fl, int ec, std::string errmsg, bg_api::rest::spot_tickers_t res) {
            if (ec) {
                std::cerr << "get spot tickers error: fl=" << fl << ", ec=" << ec << ", errmsg=" << errmsg << std::endl;
                return false;
            }

            std::cout << "spot tickers" << std::endl;

            return true;
        }
    );

    api.getSpotTrades("BTCUSDT",
        [](const char *fl, int ec, std::string errmsg, bg_api::rest::trades_t res) {
            if (ec) {
                std::cerr << "get spot trades error: fl=" << fl << ", ec=" << ec << ", errmsg=" << errmsg << std::endl;
                return false;
            }

            std::cout << "spot trades BTCUSDT" << std::endl;

            return true;
        }
    );

    api.getSpotTrades("BTCUSDT_SPBL",
        [](const char *fl, int ec, std::string errmsg, bg_api::rest::trades_t res) {
            if (ec) {
                std::cerr << "get spot trades error: fl=" << fl << ", ec=" << ec << ", errmsg=" << errmsg << std::endl;
                return false;
            }

            std::cout << "spot trades BTCUSDT_SPBL" << std::endl;

            return true;
        }
    );

    api.getSpotCandles("BTCUSDT", 60,
        [](const char *fl, int ec, std::string errmsg, bg_api::rest::candles_t res) {
            if (ec) {
                std::cerr << "get spot candles error: fl=" << fl << ", ec=" << ec << ", errmsg=" << errmsg << std::endl;
                return false;
            }

            std::cout << "spot candles BTCUSDT" << std::endl;

            return true;
        }
    );

    api.getSpotCandles("BTCUSDT_SPBL", 60,
        [](const char *fl, int ec, std::string errmsg, bg_api::rest::candles_t res) {
            if (ec) {
                std::cerr << "get spot candles error: fl=" << fl << ", ec=" << ec << ", errmsg=" << errmsg << std::endl;
                return false;
            }

            std::cout << "spot candles BTCUSDT_SPBL" << std::endl;

            return true;
        }
    );

    api.getSpotDepth("BTCUSDT",
        [](const char *fl, int ec, std::string errmsg, bg_api::rest::depth_t res) {
            if (ec) {
                std::cerr << "get spot depth error: fl=" << fl << ", ec=" << ec << ", errmsg=" << errmsg << std::endl;
                return false;
            }

            std::cout << "spot depth BTCUSDT" << std::endl;

            return true;
        }
    );

    api.getSpotDepth("BTCUSDT_SPBL",
        [](const char *fl, int ec, std::string errmsg, bg_api::rest::depth_t res) {
            if (ec) {
                std::cerr << "get spot depth error: fl=" << fl << ", ec=" << ec << ", errmsg=" << errmsg << std::endl;
                return false;
            }

            std::cout << "spot depth BTCUSDT_SPBL" << std::endl;

            return true;
        }
    );
    
    api.withdrawList("USDT", 1640995200000, 1672531200000,
        [](const char *fl, int ec, std::string errmsg, bg_api::rest::deposit_withdrawals_t res) {
            if (ec) {
                std::cerr << "get withdraw list error: fl=" << fl << ", ec=" << ec << ", errmsg=" << errmsg << std::endl;
                return false;
            }

            std::cout << "withdraw list USDT" << std::endl;

            return true;
        }
    );

    api.depositList("USDT", 1640995200000, 1672531200000,
        [](const char *fl, int ec, std::string errmsg, bg_api::rest::deposit_withdrawals_t res) {
            if (ec) {
                std::cerr << "get deposit list error: fl=" << fl << ", ec=" << ec << ", errmsg=" << errmsg << std::endl;
                return false;
            }

            std::cout << "deposit list USDT" << std::endl;

            return true;
        }
    );

    api.getApiKeyInfo(
        [](const char *fl, int ec, std::string errmsg, bg_api::rest::apikey_t res) {
            if (ec) {
                std::cerr << "get api key info error: fl=" << fl << ", ec=" << ec << ", errmsg=" << errmsg << std::endl;
                return false;
            }

            std::cout << "api key info" << std::endl;
            
            return true;
        }
    );

    api.getSpotAccount("USDT",
        [](const char *fl, int ec, std::string errmsg, bg_api::rest::spot_account_t res) {
            if (ec) {
                std::cerr << "get account error: fl=" << fl << ", ec=" << ec << ", errmsg=" << errmsg << std::endl;
                return false;
            }

            std::cout << "account" << std::endl;

            return true;
        }
    );

    api.getBills(1, bg_api::_group_type::_deposit, bg_api::_biz_type::_deposit,
        [](const char *fl, int ec, std::string errmsg, bg_api::rest::bills_t res) {
            if (ec) {
                std::cerr << "get bills error: fl=" << fl << ", ec=" << ec << ", errmsg=" << errmsg << std::endl;
                return false;
            }

            std::cout << "bills" << std::endl;

            return true;
        }
    );

    api.getTransferList(1, "exchange",
        [](const char *fl, int ec, std::string errmsg, bg_api::rest::transfers_t res) {
            if (ec) {
                std::cerr << "get transfer list error: fl=" << fl << ", ec=" << ec << ", errmsg=" << errmsg << std::endl;
                return false;
            }

            std::cout << "transfer list" << std::endl;

            return true;
        }
    );

    api.orderHistory("BTCUSDT",
        [](const char *fl, int ec, std::string errmsg, bg_api::rest::spot_orders_t res) {
            if (ec) {
                std::cerr << "get order history error: fl=" << fl << ", ec=" << ec << ", errmsg=" << errmsg << std::endl;
                return false;
            }

            std::cout << "order history BTCUSDT" << std::endl;

            return true;
        }
    );

    ioctx.run();

    return 0;
}

/*************************************************************************************************/
