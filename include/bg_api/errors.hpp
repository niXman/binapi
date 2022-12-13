
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

#ifndef __bg_api__errors_hpp
#define __bg_api__errors_hpp

#include <string>
#include <utility>

namespace flatjson {
struct fjson;
} // ns flatjson

namespace bg_api {
namespace rest {

/*************************************************************************************************/

// https://github.com/binance/binance-spot-api-docs/blob/master/errors.md
    enum class error : int {
        OK = 0,
        EMPTY_ACCESS_KEY = 40001,
        EMPTY_ACCESS_SIGN = 40002,
        EMPTY_ACCESS_TIMESTAMP = 40003,
        INVALID_ACCESS_TIMESTAMP = 40005,
        INVALID_ACCESS_KEY = 40006,
        INVALID_CONTENT_TYPE = 40007,
        TIMESTAMP_EXPIRED = 40008,
        API_VERIFICATION_FAILED = 40009,
        REQUEST_TOO_FREQUENT = 429,
        EMPTY_ACCESS_PASSPHRASE = 40011,
        INCORRECT_APIKEY_PASSPHRASE = 40012,
        USER_FROZEN = 40013,
        INCORRECT_PERMISSIONS = 40014,
        SYSTEM_ERROR = 40015,
        BIND_PHONE_GOOGLE = 40016,
        PARAMETER_VERIFICATION_FAILED = 40017,
        ILLEGAL_IP_REQUEST = 40018,
        TP_GREATER_1 = 43013,
        TP_GREATER_2 = 43014,
        SL_LESS_1 = 43015,
        SL_LESS_2 = 43016,
        NO_LIQUIDATION_LIMIT = 43001,
        TP_SL_DNE = 43020,
        TP_SL_CLOSED = 43021,
        FAILED_DEFAULT_SL = 43022,
        INSUFFICIENT_POSITION = 43023,
        TP_SL_IN_EXISTING = 43024,
        LIMIT_ORDER_DNE = 43025,
        LIMIT_ORDER_CLOSED = 43026,
        TOO_MANY_OPEN = 45116,
        PLEASE_CLOSE_POSITION = 40728,
        DUPLICATE_CLIENT_OID = 40757,
        NOT_ENOUGH_POSITION_AVAIL = 40786,
        SERVICE_ERROR = 40725,
        REQUEST_TIMEOUT = 40010
    };
    const char* error_to_string(error v);
    const char* error_to_string(int v);
    inline bool error_equal(int v, error err) { return v == static_cast<int>(err); }

/*************************************************************************************************/

    bool is_api_error(const flatjson::fjson &json);

    std::pair<int, std::string>
    construct_error(const flatjson::fjson &json);

/*************************************************************************************************/

} // ns rest

namespace ws {
    enum class error : int {
        OK = 0,
        CHANNEL_DNE = 30001,
        ILLEGAL_REQUEST = 30002,
        INVALID_OP = 30003,
        NEED_LOGIN = 30004,
        LOGIN_FAILED = 30005,
        INVALID_ACCESS_KEY = 30011,
        INVALID_ACCESS_PASSPHRASE = 30012,
        INVALID_ACCESS_TIMESTAMP = 30013,
        REQUEST_TIMESTAMP_EXPIRED = 30014,
        INVALID_SIGNATURE = 30015
    };
    const char* error_to_string(error v);
    const char* error_to_string(int v);
    inline bool error_equal(int v, error err) { return v == static_cast<int>(err); }

/*************************************************************************************************/

    bool is_api_error(const flatjson::fjson &json);

    std::pair<int, std::string>
    construct_error(const flatjson::fjson &json);

/*************************************************************************************************/

} // ns ws

} // ns bg_api

#endif // __bg_api__errors_hpp
