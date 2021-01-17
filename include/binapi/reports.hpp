
// ----------------------------------------------------------------------------
//                              Apache License
//                        Version 2.0, January 2004
//                     http://www.apache.org/licenses/
//
// This file is part of binapi(https://github.com/niXman/binapi) project.
//
// Copyright (c) 2019-2021 niXman (github dot nixman dog pm.me). All rights reserved.
// ----------------------------------------------------------------------------

#ifndef __binapi__reports_hpp
#define __binapi__reports_hpp

#include <iosfwd>
#include <string>
#include <vector>
#include <functional>

namespace binapi {
namespace rest {

struct api;
struct account_info_t;
struct exchange_info_t;
struct order_info_t;
struct orders_info_t;

} // ns rest

/*************************************************************************************************/

void make_balance_report(
     std::ostream &os
    ,rest::api &api
    ,const rest::account_info_t &accinfo
    ,const rest::exchange_info_t &exinfo
);

rest::orders_info_t get_open_orders(
     rest::api &api
    ,const rest::exchange_info_t &exinfo
    ,const std::vector<std::string> &pairs
    ,const std::function<void(const std::string &)> &tick = {}
    ,const char *side = nullptr // "BUY"/"SELL"
    ,const std::uint64_t start_time = 0
    ,const std::string &start_time_str = ""
);

void make_open_orders_report(
     std::ostream &os
    ,rest::api &api
    ,const rest::exchange_info_t &exinfo
    ,const std::vector<std::string> &pairs
    ,const std::function<void(const std::string &)> &tick = {}
    ,const std::uint64_t start_time = 0
    ,const std::string &start_time_str = ""
);

void make_trades_report(
     std::ostream &os
    ,rest::api &api
    ,const rest::account_info_t &accinfo
    ,const rest::exchange_info_t &exinfo
    ,const std::vector<std::string> &pairs
    ,const std::function<void(const rest::order_info_t &)> &tick = {}
    ,const std::uint64_t start_time = 0
    ,const std::string &start_time_str = ""
);

void make_trades_report_for_last_day(
     std::ostream &os
    ,rest::api &api
    ,const rest::account_info_t &accinfo
    ,const rest::exchange_info_t &exinfo
    ,const std::vector<std::string> &pairs
    ,const std::function<void(const rest::order_info_t &)> &tick = {}
);

void show_exchanger_price_for_orders(
     std::ostream &os
    ,rest::api &api
    ,const rest::exchange_info_t &exinfo
    ,const std::vector<std::string> &pairs
    ,const std::function<void(const std::string &)> &tick = {}
);

void calc_loss_for_orders(
     std::ostream &os
    ,rest::api &api
    ,const rest::exchange_info_t &exinfo
    ,const std::vector<std::string> &pairs
    ,const std::function<void(const std::string &)> &tick = {}
);

/*************************************************************************************************/

} // ns binapi

#endif // __binapi__reports_hpp
