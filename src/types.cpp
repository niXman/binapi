
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

#include <bg_api/types.hpp>
#include <bg_api/flatjson.hpp>
#include <bg_api/fnv1a.hpp>
#include <bg_api/LUTs.hpp>

#include <type_traits>

//#include <iostream> // TODO: comment out

namespace bg_api {

//------------------------------------------------------------------------------

template<typename T>
typename std::enable_if<std::is_integral<T>::value>::type
__get_json(T &v, const char *member, const flatjson::fjson &j) {
    v = j.at(member).to<T>();
}

template<typename T>
typename std::enable_if<std::is_same<T, std::string>::value>::type
__get_json(T &v, const char *member, const flatjson::fjson &j) {
    const auto &o = j.at(member);
    v = (o.is_null() ? std::string{} : o.to_string());
}

template<typename T>
typename std::enable_if<std::is_same<T, double_type>::value>::type
__get_json(T &v, const char *member, const flatjson::fjson &j) {
    v.assign(j.at(member).to_string());
}

#define __bg_api_GET2(obj, member, json) \
    __get_json(obj.member, #member, json)

#define __bg_api_GET(member) __bg_api_GET2(res, member, json)

//------------------------------------------------------------------------------

namespace rest {

//------------------------------------------------------------------------------

server_time_t server_time_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());
    assert(json.is_object());

    server_time_t res{};
    __bg_api_GET(serverTime);
    __bg_api_GET(reqTime);

    return res;
}

std::ostream &operator<<(std::ostream &os, const server_time_t &f) {
    os << "{\n" << "\t\"serverTime\":" << f.serverTime << ",\n"
    << "\t\"requestTime\":" << f.reqTime << "\n}";

    return os;
}

//------------------------------------------------------------------------------

coin_t::chain_t coin_t::chain_t::construct(const flatjson::fjson &json) {
    (void)json;

    static const coin_t::chain_t res{};

    return res;
}

std::ostream &operator<<(std::ostream &os, const coin_t::chain_t &f) {
    os << "\t\t{\n"
    << "\t\t\t\"chain\":" << f.chain << ",\n"
    << "\t\t\t\"needTag\":" << std::boolalpha << f.needTag << ",\n"
    << "\t\t\t\"withdrawAble\":" << std::boolalpha << f.withdrawAble << ",\n"
    << "\t\t\t\"rechargeAble\":" << std::boolalpha << f.rechargeAble << ",\n"
    << "\t\t\t\"withdrawFee\":" << f.withdrawFee << ",\n"
    << "\t\t\t\"depositConfirm\":" << f.depositConfirm << ",\n"
    << "\t\t\t\"withdrawConfirm\":" << f.withdrawConfirm << ",\n"
    << "\t\t\t\"minDepositAmount\":" << f.minDepositAmount << ",\n"
    << "\t\t\t\"minWithdrawAmount\":" << f.minWithdrawAmount << ",\n"
    << "\t\t\t\"browseUrl\":" << f.browseUrl << "\n\t}";

    return os;
}

coin_t coin_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    coin_t res{};
    __bg_api_GET(coinId);
    __bg_api_GET(coinName);
    __bg_api_GET(transfer);
    const auto arr = json.at("chains");
    assert(arr.is_array());
    for (auto idx = 0; idx < arr.size(); ++idx) {
        coin_t::chain_t item{};
        const auto it = arr.at(idx);
        __bg_api_GET2(item, chain, it);
        __bg_api_GET2(item, needTag, it);
        __bg_api_GET2(item, withdrawAble, it);
        __bg_api_GET2(item, rechargeAble, it);
        __bg_api_GET2(item, withdrawFee, it);
        __bg_api_GET2(item, depositConfirm, it);
        __bg_api_GET2(item, withdrawConfirm, it);
        __bg_api_GET2(item, minDepositAmount, it);
        __bg_api_GET2(item, minWithdrawAmount, it);
        __bg_api_GET2(item, browseUrl, it);
    }

    return res;
}

std::ostream &operator<<(std::ostream &os, const coin_t &f) {
    os << "{\n"
    << "\t\"coinId\":" << f.coinId << ",\n"
    << "\t\"coinName\":" << f.coinName << ",\n"
    << "\t\"transfer\":" << std::boolalpha << f.transfer << ",\n"
    << "\t\"chains\":[\n";
    for (auto it = f.chains.begin(); it != f.chains.end(); ++it) {
        os << *it;
        if (std::next(it) != f.chains.end()) {
            os << ",\n";
        }
    }

    os << "\n\t]\n}";

    return os;
}

//------------------------------------------------------------------------------

symbol_t symbol_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());


    const auto arr = json.at("data");
    for (auto idx = 0; idx < arr.size(); ++idx) {

    }
}

std::ostream &operator<<(std::ostream &os, const symbol_t &f) {

}

//------------------------------------------------------------------------------

ticker_t ticker_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const ticker_t &f) {

}

//------------------------------------------------------------------------------

trade_t trade_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const trade_t &f) {

}

//------------------------------------------------------------------------------

candle_t candle_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const candle_t &f) {

}

//------------------------------------------------------------------------------

depth_t depth_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const depth_t &f) {

}

//------------------------------------------------------------------------------

contract_t contract_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const contract_t &f) {

}

//------------------------------------------------------------------------------

fill_t fill_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const fill_t &f) {

}

//------------------------------------------------------------------------------

leverage_t leverage_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const leverage_t &f) {

}

//------------------------------------------------------------------------------

address_t address_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const address_t &f) {

}

//------------------------------------------------------------------------------

deposit_withdrawal_t deposit_withdrawal_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const deposit_withdrawal_t &f) {

}

//------------------------------------------------------------------------------

apikey_t apikey_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const apikey_t &f) {

}

//------------------------------------------------------------------------------

spot_account_t spot_account_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const spot_account_t &f) {

}

//------------------------------------------------------------------------------

bill_t bill_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const bill_t &f) {

}

//------------------------------------------------------------------------------

transfer_t transfer_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const transfer_t &f) {

}

//------------------------------------------------------------------------------

futures_account_t futures_account_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const futures_account_t &f) {

}

//------------------------------------------------------------------------------

position_t position_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const position_t &f) {

}

//------------------------------------------------------------------------------

futures_bill_t futures_bill_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const futures_bill_t &f) {

}

//------------------------------------------------------------------------------

coin_leverage_t coin_leverage_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const coin_leverage_t &f) {

}

//------------------------------------------------------------------------------

place_spot_order_t place_spot_order_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const place_spot_order_t &f) {

}

//------------------------------------------------------------------------------

place_futures_order_t place_futures_order_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const place_futures_order_t &f) {

}

//------------------------------------------------------------------------------

spot_order_t spot_order_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const spot_order_t &f) {

}

//------------------------------------------------------------------------------

futures_order_t futures_order_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const futures_order_t &f) {

}

//------------------------------------------------------------------------------

transaction_t transaction_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const transaction_t &f) {

}

//------------------------------------------------------------------------------

place_plan_order_spot_t place_plan_order_spot_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const place_plan_order_spot_t &f) {

}

//------------------------------------------------------------------------------

place_plan_order_futures_t place_plan_order_futures_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const place_plan_order_futures_t &f) {

}

//------------------------------------------------------------------------------

spot_plan_order_t spot_plan_order_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const spot_plan_order_t &f) {

}

//------------------------------------------------------------------------------

futures_plan_order_t futures_plan_order_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const futures_plan_order_t &f) {

}

//------------------------------------------------------------------------------

futures_fill_t futures_fill_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const futures_fill_t &f) {

}

//------------------------------------------------------------------------------

trader_order_t trader_order_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const trader_order_t &f) {

}

//------------------------------------------------------------------------------

follower_order_t follower_order_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const follower_order_t &f) {

}

//------------------------------------------------------------------------------

trader_profit_t trader_profit_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const trader_profit_t &f) {

}

//------------------------------------------------------------------------------

profit_token_t profit_token_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const profit_token_t &f) {

}

//------------------------------------------------------------------------------

trader_symbols_t trader_symbols_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const trader_symbols_t &f) {

}

//------------------------------------------------------------------------------

subaccount_t subaccount_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const subaccount_t &f) {

}

//------------------------------------------------------------------------------

sub_spot_asset_t sub_spot_asset_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const sub_spot_asset_t &f) {

}

//------------------------------------------------------------------------------

sub_futures_asset_t sub_futures_asset_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const sub_futures_asset_t &f) {

}

//------------------------------------------------------------------------------

sub_address_t sub_address_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const sub_address_t &f) {

}

//------------------------------------------------------------------------------

sub_apikey_t sub_apikey_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const sub_apikey_t &f) {

}

} // ns rest

namespace ws {

//------------------------------------------------------------------------------

spot_ticker_t spot_ticker_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const spot_ticker_t &f) {

}

//------------------------------------------------------------------------------

futures_ticker_t futures_ticker_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const futures_ticker_t &f) {

}

//------------------------------------------------------------------------------

candle_t candle_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const candle_t &f) {

}

//------------------------------------------------------------------------------

depth_t depth_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const depth_t &f) {

}

//------------------------------------------------------------------------------

trade_t trade_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const trade_t &f) {

}

//------------------------------------------------------------------------------

spot_account_t spot_account_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const spot_account_t &f) {

}

//------------------------------------------------------------------------------

futures_account_t futures_account_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const futures_account_t &f) {

}

//------------------------------------------------------------------------------

position_t position_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const position_t &f) {

}

//------------------------------------------------------------------------------

spot_order_t spot_order_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const spot_order_t &f) {

}

//------------------------------------------------------------------------------

futures_order_t futures_order_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const futures_order_t &f) {

}

//------------------------------------------------------------------------------

plan_order_t plan_order_t::construct(const flatjson::fjson &json) {

}

std::ostream &operator<<(std::ostream &os, const plan_order_t &f) {

}

//------------------------------------------------------------------------------

} // ns ws

} // ns bg_api
