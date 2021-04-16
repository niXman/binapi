
// ----------------------------------------------------------------------------
//                              Apache License
//                        Version 2.0, January 2004
//                     http://www.apache.org/licenses/
//
// This file is part of binapi(https://github.com/niXman/binapi) project.
//
// Copyright (c) 2019-2021 niXman (github dot nixman dog pm.me). All rights reserved.
// ----------------------------------------------------------------------------

#ifndef __binapi__errors_hpp
#define __binapi__errors_hpp

#include <string>
#include <utility>

namespace flatjson {
struct fjson;
} // ns flatjson

namespace binapi {
namespace rest {

/*************************************************************************************************/

// https://github.com/binance/binance-spot-api-docs/blob/master/errors.md
enum class e_error: int {
     OK = 0
    ,UNKNOWN = -1000
    ,DISCONNECTED = -1001
    ,UNAUTHORIZED = -1002
    ,TOO_MANY_REQUESTS = -1003
    ,UNEXPECTED_RESP = -1006
    ,TIMEOUT = -1007
    ,UNKNOWN_ORDER_COMPOSITION = -1014
    ,TOO_MANY_ORDERS = -1015
    ,SERVICE_SHUTTING_DOWN = -1016
    ,UNSUPPORTED_OPERATION = -1020
    ,INVALID_TIMESTAMP = -1021
    ,INVALID_SIGNATURE = -1022
    ,ILLEGAL_CHARS = -1100
    ,TOO_MANY_PARAMETERS = -1101
    ,MANDATORY_PARAM_EMPTY_OR_MALFORMED = -1102
    ,UNKNOWN_PARAM = -1103
    ,UNREAD_PARAMETERS = -1104
    ,PARAM_EMPTY = -1105
    ,PARAM_NOT_REQUIRED = -1106
    ,BAD_PRECISION = -1111
    ,NO_DEPTH = -1112
    ,TIF_NOT_REQUIRED = -1114
    ,INVALID_TIF = -1115
    ,INVALID_ORDER_TYPE = -1116
    ,INVALID_SIDE = -1117
    ,EMPTY_NEW_CL_ORD_ID = -1118
    ,EMPTY_ORG_CL_ORD_ID = -1119
    ,BAD_INTERVAL = -1120
    ,BAD_SYMBOL = -1121
    ,INVALID_LISTEN_KEY = -1125
    ,MORE_THAN_XX_HOURS = -1127
    ,OPTIONAL_PARAMS_BAD_COMBO = -1128
    ,INVALID_PARAMETER = -1130
    ,NEW_ORDER_REJECTED = -2010
    ,CANCEL_REJECTED = -2011
    ,NO_SUCH_ORDER = -2013
    ,BAD_API_KEY_FMT = -2014
    ,REJECTED_MBX_KEY = -2015
    ,NO_TRADING_WINDOW = -2016
};

const char* e_error_to_string(e_error v);
const char* e_error_to_string(int v);

inline bool e_error_equal(int v, e_error err) { return v == static_cast<int>(err); }

/*************************************************************************************************/

bool is_api_error(const flatjson::fjson &json);

std::pair<int, std::string>
construct_error(const flatjson::fjson &json);

/*************************************************************************************************/

} // ns rest
} // ns binapi

#endif // __binapi__errors_hpp
