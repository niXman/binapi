
// ----------------------------------------------------------------------------
//                              Apache License
//                        Version 2.0, January 2004
//                     http://www.apache.org/licenses/
//
// This file is part of binapi(https://github.com/niXman/binapi) project.
//
// Copyright (c) 2019-2021 niXman (github dot nixman dog pm.me). All rights reserved.
// ----------------------------------------------------------------------------

#include <binapi/errors.hpp>
#include <binapi/flatjson.hpp>

namespace binapi {
namespace rest {

/*************************************************************************************************/

const char* e_error_to_string(e_error v) {
    switch ( v ) {
        case e_error::OK : return "OK";
        case e_error::UNKNOWN : return "UNKNOWN";
        case e_error::DISCONNECTED : return "DISCONNECTED";
        case e_error::UNAUTHORIZED : return "UNAUTHORIZED";
        case e_error::TOO_MANY_REQUESTS : return "TOO_MANY_REQUESTS";
        case e_error::UNEXPECTED_RESP : return "UNEXPECTED_RESP";
        case e_error::TIMEOUT : return "TIMEOUT";
        case e_error::UNKNOWN_ORDER_COMPOSITION : return "UNKNOWN_ORDER_COMPOSITION";
        case e_error::TOO_MANY_ORDERS : return "TOO_MANY_ORDERS";
        case e_error::SERVICE_SHUTTING_DOWN : return "SERVICE_SHUTTING_DOWN";
        case e_error::UNSUPPORTED_OPERATION : return "UNSUPPORTED_OPERATION";
        case e_error::INVALID_TIMESTAMP : return "INVALID_TIMESTAMP";
        case e_error::INVALID_SIGNATURE : return "INVALID_SIGNATURE";
        case e_error::ILLEGAL_CHARS : return "ILLEGAL_CHARS";
        case e_error::TOO_MANY_PARAMETERS : return "TOO_MANY_PARAMETERS";
        case e_error::MANDATORY_PARAM_EMPTY_OR_MALFORMED : return "MANDATORY_PARAM_EMPTY_OR_MALFORMED";
        case e_error::UNKNOWN_PARAM : return "UNKNOWN_PARAM";
        case e_error::UNREAD_PARAMETERS : return "UNREAD_PARAMETERS";
        case e_error::PARAM_EMPTY : return "PARAM_EMPTY";
        case e_error::PARAM_NOT_REQUIRED : return "PARAM_NOT_REQUIRED";
        case e_error::BAD_PRECISION : return "BAD_PRECISION";
        case e_error::NO_DEPTH : return "NO_DEPTH";
        case e_error::TIF_NOT_REQUIRED : return "TIF_NOT_REQUIRED";
        case e_error::INVALID_TIF : return "INVALID_TIF";
        case e_error::INVALID_ORDER_TYPE : return "INVALID_ORDER_TYPE";
        case e_error::INVALID_SIDE : return "INVALID_SIDE";
        case e_error::EMPTY_NEW_CL_ORD_ID : return "EMPTY_NEW_CL_ORD_ID";
        case e_error::EMPTY_ORG_CL_ORD_ID : return "EMPTY_ORG_CL_ORD_ID";
        case e_error::BAD_INTERVAL : return "BAD_INTERVAL";
        case e_error::BAD_SYMBOL : return "BAD_SYMBOL";
        case e_error::INVALID_LISTEN_KEY : return "INVALID_LISTEN_KEY";
        case e_error::MORE_THAN_XX_HOURS : return "MORE_THAN_XX_HOURS";
        case e_error::OPTIONAL_PARAMS_BAD_COMBO : return "OPTIONAL_PARAMS_BAD_COMBO";
        case e_error::INVALID_PARAMETER : return "INVALID_PARAMETER";
        case e_error::NEW_ORDER_REJECTED : return "NEW_ORDER_REJECTED";
        case e_error::CANCEL_REJECTED : return "CANCEL_REJECTED";
        case e_error::NO_SUCH_ORDER : return "NO_SUCH_ORDER";
        case e_error::BAD_API_KEY_FMT : return "BAD_API_KEY_FMT";
        case e_error::REJECTED_MBX_KEY : return "REJECTED_MBX_KEY";
        case e_error::NO_TRADING_WINDOW : return "NO_TRADING_WINDOW";
    }

    return "UNKNOWN_ERROR";
}

const char* e_error_to_string(int v) {
    return e_error_to_string(static_cast<e_error>(v));
}

/*************************************************************************************************/

bool is_api_error(const flatjson::fjson &json) {
    return json.contains("code") && json.contains("msg");
}

/*************************************************************************************************/

std::pair<int, std::string>
construct_error(const flatjson::fjson &json) {
    auto ec = json.at("code").to_int();
    auto msg = json.at("msg").to_string();

    return std::make_pair(ec, std::move(msg));
}

/*************************************************************************************************/

} // ns rest
} // ns binapi
