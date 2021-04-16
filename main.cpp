
// ----------------------------------------------------------------------------
//                              Apache License
//                        Version 2.0, January 2004
//                     http://www.apache.org/licenses/
//
// This file is part of binapi(https://github.com/niXman/binapi) project.
//
// Copyright (c) 2019-2021 niXman (github dot nixman dog pm.me). All rights reserved.
// ----------------------------------------------------------------------------

#include "binapi/websocket.hpp"
#include "binapi/api.hpp"
#include "binapi/pairslist.hpp"
#include "binapi/reports.hpp"
#include "binapi/flatjson.hpp"

#include <boost/asio/io_context.hpp>
#include <boost/asio/steady_timer.hpp>

#include <iostream>
#include <fstream>
#include <binapi/errors.hpp>

/*************************************************************************************************/

std::string read_file(const char *fname) {
    std::ifstream file(fname);
    assert(file);

    std::string res{
         (std::istreambuf_iterator<char>(file))
        ,(std::istreambuf_iterator<char>())
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
    assert(argc == 3);
    const std::string pk = argv[1];
    const std::string sk = argv[2];

    std::cout.precision(8);

    boost::asio::io_context ioctx;
    binapi::ws::websockets wsp(ioctx, "stream.binance.com", "9443");

    binapi::rest::api api(
         ioctx
        ,"api.binance.com"
        ,"443"
        ,pk
        ,sk
        ,10000
    );

    static const char *testpair = "BNBUSDT";

    /** */
    {
        const std::string accinfo_str = read_file("accinfo.json");
        const std::string exinfo_str = read_file("exinfo.json");

        const flatjson::fjson exinfo_json{exinfo_str.c_str(), exinfo_str.size()};
        binapi::rest::exchange_info_t exinfo = binapi::rest::exchange_info_t::construct(exinfo_json);

        const flatjson::fjson accinfo_json{accinfo_str.c_str(), accinfo_str.size()};
        binapi::rest::account_info_t accinfo = binapi::rest::account_info_t::construct(accinfo_json);

        auto pairs0 = binapi::process_pairs("BTCUSDT", "", exinfo);
        assert(pairs0.size() == 1 && *(pairs0.begin()) == "BTCUSDT");

        auto pairs1 = binapi::process_pairs("BTCUSDT,BNBUSDT", "BTCUSDT", exinfo);
        assert(pairs1.size() == 1 && *(pairs1.begin()) == "BNBUSDT");

        auto pairs2 = binapi::process_pairs("*USDT", "BTCUSDT,BNBUSDT", exinfo);
        assert(pairs2.size() == 156 && pairs2.count("BTCUSDT") == 0 && pairs2.count("BNBUSDT") == 0);

        static const auto trades_report_cb = [](const binapi::rest::order_info_t &o){
            std::cout << o.symbol << " - " << o.orderId << std::endl;
        };
        std::cout << "******************** BALANCE REPORT *********************************" << std::endl;
        binapi::make_balance_report(std::cout, api, accinfo, exinfo);
        std::cout << std::endl;
        std::cout << "********************* TRADES REPORT *********************************" << std::endl;
//        binapi::make_trades_report(std::cout, api, accinfo, exinfo, {"ETHUSDT"}, trades_report_cb);
//        binapi::make_trades_report(std::cout, api, accinfo, exinfo, {"*USDT"}, trades_report_cb, 0, "2018-11-12 17:38:29"); // "2018-11-12 17:38:29"
        binapi::make_trades_report_for_last_day(std::cout, api, accinfo, exinfo, {"ETHUSDT"}, trades_report_cb);

        std::cout << std::endl;
        std::cout << "******************* OPEN ORDERS REPORT ******************************" << std::endl;
        binapi::make_open_orders_report(std::cout, api, exinfo, {});

        std::cout << std::endl;
    }
    /** */

    auto account = api.account_info();
    if ( ! binapi::rest::e_error_equal(account.ec, binapi::rest::e_error::OK) ) {
        std::cout
        << "account_error: ec=" << account.ec
        << ", ename=" << binapi::rest::e_error_to_string(account.ec)
        << ", emsg=" << account.errmsg
        << std::endl << std::endl;

        return EXIT_FAILURE;
    } else {
        BREAK_IF_ERROR(account);
        std::cout << "account=" << account.v << std::endl << std::endl;
    }

    auto exinfo = api.exchange_info();
    BREAK_IF_ERROR(exinfo);
    std::cout << "exinfo=" << exinfo.v << std::endl << std::endl;

    auto ping = api.ping();
    BREAK_IF_ERROR(ping);
    std::cout << "ping=" << ping.v.ok << std::endl << std::endl;

    auto server_time = api.server_time();
    BREAK_IF_ERROR(server_time);
    std::cout << "server_time=" << server_time.v.serverTime << std::endl << std::endl;

    auto price = api.price(testpair);
    BREAK_IF_ERROR(price);
    std::cout << "price=" << price.v.price << std::endl << std::endl;

    auto prices = api.prices();
    BREAK_IF_ERROR(price);
    std::cout << "prices=" << prices.v << std::endl << std::endl;

    auto depths = api.depths(testpair, 5);
    BREAK_IF_ERROR(depths);
    std::cout << "depths=" << depths.v << std::endl << std::endl;

    auto trade = api.trade(testpair);
    BREAK_IF_ERROR(trade);
    std::cout << "trade=" << trade.v << std::endl << std::endl;

    auto trades = api.trades(testpair, 2);
    BREAK_IF_ERROR(trades);
    std::cout << "trades=" << trades.v << std::endl << std::endl;

    auto agg_trade = api.agg_trade(testpair);
    BREAK_IF_ERROR(agg_trade);
    std::cout << "agg_trade=" << agg_trade.v << std::endl << std::endl;

    auto agg_trades = api.agg_trades(testpair, 2);
    BREAK_IF_ERROR(agg_trades);
    std::cout << "agg_trades=" << agg_trades.v << std::endl << std::endl;

    auto klines = api.klines(testpair, "1m", 10);
    BREAK_IF_ERROR(klines);
    std::cout << "klines=" << klines.v << std::endl << std::endl;

    auto orderinfo = api.order_info("NULSUSDT", 3040219);
    BREAK_IF_ERROR(orderinfo);
    std::cout << "orderinfo=" << orderinfo.v << std::endl << std::endl;

    auto openorders = api.open_orders(testpair);
    BREAK_IF_ERROR(openorders);
    std::cout << "openorders=" << openorders.v << std::endl << std::endl;

    auto allorders = api.all_orders(testpair, 0, 0, 0, 5);
    BREAK_IF_ERROR(allorders);
    std::cout << "allorders=" << allorders.v << std::endl << std::endl;

    auto mytrades = api.my_trades(testpair, 0, 0, 0, 5);
    BREAK_IF_ERROR(mytrades);
    std::cout << "mytrades=" << mytrades.v << std::endl << std::endl;

//    auto neworder = api.new_order(
//         "BNBUSDT"
//        ,binapi::e_side::buy
//        ,binapi::e_type::limit
//        ,binapi::e_time::GTC
//        ,binapi::e_trade_resp_type::FULL
//        ,"0.1"
//        ,"0"
//        ,nullptr
//        ,nullptr
//        ,nullptr
//    );
//    BREAK_IF_ERROR(neworder);
//    std::cout << "neworder=" << neworder.v << std::endl << std::endl;

//    auto cancelorder = api.cancel_order("BNBUSDT", 1, nullptr, nullptr);
//    BREAK_IF_ERROR(cancelorder);
//    std::cout << "cancelorder=" << cancelorder.v << std::endl << std::endl;
//
    auto start_uds = api.start_user_data_stream();
    BREAK_IF_ERROR(start_uds);
    std::cout << "start_uds=" << start_uds.v << std::endl << std::endl;

    auto user_data_stream = wsp.userdata(start_uds.v.listenKey.c_str(),
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
    wsp.diff_depth(testpair, binapi::e_freq::_100ms,
        [](const char *fl, int ec, std::string errmsg, binapi::ws::diff_depths_t msg) -> bool {
            if ( ec ) {
                std::cout << "subscribe_depth(): fl=" << fl << ", ec=" << ec << ", errmsg: " << errmsg << ", msg: " << msg << std::endl;
                return false;
            }

            std::cout << "depth: " << msg << std::endl;
            return true;
        }
    );
    wsp.trade(testpair,
        [](const char *fl, int ec, std::string errmsg, binapi::ws::trade_t msg) -> bool {
            if ( ec ) {
                std::cout << "subscribe_trade(): fl=" << fl << ", ec=" << ec << ", errmsg: " << errmsg << ", msg: " << msg << std::endl;
                return false;
            }

            std::cout << "trade: " << msg << std::endl;
            return true;
        }
    );

    boost::asio::steady_timer unsubscribe_timer{ioctx};
    unsubscribe_timer.expires_after(std::chrono::seconds{20});
    unsubscribe_timer.async_wait(
        [user_data_stream, &wsp]
        (const boost::system::error_code &) {
            std::cout << "unsubscribe userdata stream" << std::endl;
            wsp.unsubscribe(user_data_stream);
        }
    );

    auto ping_uds = api.ping_user_data_stream(start_uds.v.listenKey);
    BREAK_IF_ERROR(ping_uds);
    std::cout << "ping_uds=" << ping_uds.v << std::endl << std::endl;

////    auto close_uds = api.close_user_data_stream(start_uds.v.listenKey);
////    BREAK_IF_ERROR(close_uds);
////    std::cout << "close_uds=" << close_uds.v << std::endl << std::endl;

    ioctx.run();

    return 0;
}

/*************************************************************************************************/
