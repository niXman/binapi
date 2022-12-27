
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

#include <bg_api/errors.hpp>
#include <iostream>

namespace bg_api {
namespace rest {

/*************************************************************************************************/

    const char* error_to_string(error v) {
        switch (v) {
            case error::OK : return "OK";
            case error::EMPTY_ACCESS_KEY : return "The request header \"ACCESS_KEY\" cannot be empty";
            case error::EMPTY_ACCESS_SIGN : return "The request header \"ACCESS_SIGN\" cannot be empty";
            case error::EMPTY_ACCESS_TIMESTAMP : return "The request header \"ACCESS_TIMESTAMP\" cannot be empty";
            case error::INVALID_ACCESS_TIMESTAMP : return "Invalid ACCESS_TIMESTAMP";
            case error::INVALID_ACCESS_KEY : return "Invalid ACCESS_KEY";
            case error::INVALID_CONTENT_TYPE : return "Invalid Content_Type, please use the \"application/json\" format";
            case error::TIMESTAMP_EXPIRED : return "Request timestamp expired";
            case error::API_VERIFICATION_FAILED : return "API verification failed";
            case error::REQUEST_TOO_FREQUENT : return "Request too frequent";
            case error::EMPTY_ACCESS_PASSPHRASE : return "The request header \"ACCESS_PASSPHRASE\" cannot be empty";
            case error::INCORRECT_APIKEY_PASSPHRASE : return "apikey/passphrase is incorrect";
            case error::USER_FROZEN : return "User is frozen";
            case error::INCORRECT_PERMISSIONS : return "Incorrect permissions";
            case error::SYSTEM_ERROR : return "System error";
            case error::BIND_PHONE_GOOGLE : return "The user must bind a mobile phone or Google";
            case error::PARAMETER_VERIFICATION_FAILED : return "Parameter verification failed";
            case error::ILLEGAL_IP_REQUEST : return "Illegal ip request";
            case error::TP_GREATER_1 : return "Take profit price needs to be > current price";
            case error::TP_GREATER_2 : return "Take profit price needs to be >> current price";
            case error::SL_LESS_1 : return "Stop loss price needs to be < current price";
            case error::SL_LESS_2 : return "Stop loss price needs to be << current price";
            case error::NO_LIQUIDATION_LIMIT : return "You are currently a trader and currently do not support liquidation through limit orders";
            case error::TP_SL_DNE : return "Take profit and stop loss order does not exist";
            case error::TP_SL_CLOSED : return "The take-profit and stop-loss order has been closed";
            case error::FAILED_DEFAULT_SL : return "Failed to trigger the default stop loss";
            case error::INSUFFICIENT_POSITION : return "Insufficient position, can not set take profit or stop loss";
            case error::TP_SL_IN_EXISTING : return "Take profit/stop loss in an existing order, please change it after canceling all";
            case error::LIMIT_ORDER_DNE : return "Limit order does not exist";
            case error::LIMIT_ORDER_CLOSED : return "The limit order has been closed";
            case error::TOO_MANY_OPEN : return "Existing positions + open orders bigger than 60";
            case error::PLEASE_CLOSE_POSITION : return "You are currently a trader, please close the position under the current order";
            case error::DUPLICATE_CLIENT_OID : return "Duplicate clientOid";
            case error::NOT_ENOUGH_POSITION_AVAIL : return "Not enough position is available";
            case error::SERVICE_ERROR : return "service return an error";
            case error::REQUEST_TIMEOUT : return "Request timed out";
        }

        return "UNKNOWN_ERROR";
    }

    const char* error_to_string(int v) {
        return error_to_string(static_cast<error>(v));
    }

/*************************************************************************************************/

    bool is_api_error(simdjson::ondemand::document &doc) {
        return doc["code"].get_string().take_value() != "00000";
    }

/*************************************************************************************************/

    std::pair<int, std::string>
    construct_error(simdjson::ondemand::document &doc) {
        auto ec = static_cast<std::string>(doc["code"].get_string().take_value());
        int err = 0;
        if (ec != "00000") {
            err = std::stoi(ec);
        }
        auto msg = static_cast<std::string>(doc["msg"].get_string().take_value());

        return std::make_pair(err, std::move(msg));
    }

/*************************************************************************************************/

} // ns rest

namespace ws {

/*************************************************************************************************/

    const char* error_to_string(error v) {
        switch (v) {
            case error::OK: return "OK";
            case error::CHANNEL_DNE: return "Channel does not exist";
            case error::ILLEGAL_REQUEST: return "Illegal request";
            case error::INVALID_OP: return "Invalid op";
            case error::NEED_LOGIN: return "User needs to log in";
            case error::LOGIN_FAILED: return "Login failed";
            case error::INVALID_ACCESS_KEY: return "Invalid ACCESS_KEY";
            case error::INVALID_ACCESS_PASSPHRASE: return "Invalid ACCESS_PASSPHRASE";
            case error::INVALID_ACCESS_TIMESTAMP: return "Invalid ACCESS_TIMESTAMP";
            case error::REQUEST_TIMESTAMP_EXPIRED: return "Request timestamp expired";
            case error::INVALID_SIGNATURE: return "Invalid signature";
        }

        return "UNKNOWN ERROR";
    }

    const char* error_to_string(int v) {
        return error_to_string(static_cast<error>(v));
    }

/*************************************************************************************************/

    bool is_api_error(simdjson::ondemand::document &doc) {
        return doc["code"].get_int64_in_string().take_value() != 0;
    }

/*************************************************************************************************/

    std::pair<int, std::string>
    construct_error(simdjson::ondemand::document &doc) {
        auto ec = (int)doc["code"].get_int64_in_string().take_value();
        auto msg = static_cast<std::string>(doc["msg"].get_string().take_value());

        return std::make_pair(ec, std::move(msg));
    }

/*************************************************************************************************/

} // ns ws

} // ns bg_api
