
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

#ifndef __bg_api__enums_hpp
#define __bg_api__enums_hpp

#include <cstdint>

namespace bg_api {

/*************************************************************************************************/

enum class e_side: std::size_t {
    buy
   ,sell
};

e_side e_side_from_string(const char *str);
const char* e_side_to_string(e_side side);

/*************************************************************************************************/

enum class e_type: std::size_t {
     limit
    ,market
    ,stop_loss
    ,stop_loss_limit
    ,take_profit
    ,take_profit_limit
    ,limit_maker
};

e_type e_type_from_string(const char *str);
const char* e_type_to_string(e_type type);

/*************************************************************************************************/

enum class e_time: std::size_t {
     GTC
    ,IOC
    ,FOK
};

e_time e_time_from_string(const char *str);
const char* e_time_to_string(e_time time);

/*************************************************************************************************/

enum class e_freq: std::size_t {
     _100ms  = 100
    ,_1000ms = 1000
};

e_freq e_freq_from_string(const char *str);
const char* e_freq_to_string(e_freq freq);

/*************************************************************************************************/

enum class e_levels: std::size_t {
     _5 = 5
    ,_10 = 10
    ,_20 = 20
};

e_levels e_levels_from_string(const char *str);
const char* e_levels_to_string(e_levels level);

/*************************************************************************************************/

enum class e_trade_resp_type: std::size_t {
     ACK
    ,RESULT
    ,FULL
    ,TEST
    ,UNKNOWN
};

e_trade_resp_type e_trade_resp_type_from_string(const char *str);
const char* e_trade_resp_type_to_string(e_trade_resp_type resp);

/*************************************************************************************************/

} // ns bg_api

#endif // __bg_api__enums_hpp
