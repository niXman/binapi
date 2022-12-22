
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

#pragma warning(disable : 4715)

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

    symbol_t res = {};
    __bg_api_GET(symbol);
    __bg_api_GET(symbolName);
    __bg_api_GET(baseCoin);
    __bg_api_GET(quoteCoin);
    __bg_api_GET(minTradeAmount);
    __bg_api_GET(maxTradeAmount);
    __bg_api_GET(takerFeeRate);
    __bg_api_GET(makerFeeRate);
    __bg_api_GET(priceScale);
    __bg_api_GET(quantityScale);

    return res;
}

std::ostream &operator<<(std::ostream &os, const symbol_t &f) {
    os << "{\n"
    << "\t\"symbol\":" << f.symbol << ",\n"
    << "\t\"symbolName\":" << f.symbolName << ",\n"
    << "\t\"baseCoin\":" << f.baseCoin << ",\n"
    << "\t\"quoteCoin\":" << f.quoteCoin << ",\n"
    << "\t\"minTradeAmount\":" << f.minTradeAmount << ",\n"
    << "\t\"maxTradeAmount\":" << f.maxTradeAmount << ",\n"
    << "\t\"takerFeeRate\":" << f.takerFeeRate << ",\n"
    << "\t\"makerFeeRate\":" << f.makerFeeRate << ",\n"
    << "\t\"priceScale\":" << f.priceScale << ",\n"
    << "\t\"quantityScale\":" << f.quantityScale << "\n}";

    return os;
}

//------------------------------------------------------------------------------

ticker_t ticker_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    ticker_t res = {};
    // First get all the parameters that are shared between spot and futures
    __bg_api_GET(symbol);
    __bg_api_GET(high24h);
    __bg_api_GET(low24h);
    __bg_api_GET(quoteVol);
    __bg_api_GET(baseVol);
    __bg_api_GET(usdtVol);
    __bg_api_GET(ts);
    __bg_api_GET(buyOne);
    __bg_api_GET(sellOne);
    __bg_api_GET(bidSz);
    __bg_api_GET(askSz);
    __bg_api_GET(openUtc0);
    // Quick way of checking if this is a spot or futures json
    if (res.symbol.find('_')) { // If there's an underscore, it's futures
        __bg_api_GET(last);
        __bg_api_GET(bestAsk);
        __bg_api_GET(bestBid);
        res.close = -1.0f;
        __bg_api_GET(priceChangePercent);
        __bg_api_GET(chgUtc);
    } else { // Otherwise, it's spot
        res.last = -1.0f;
        res.bestAsk = -1.0f;
        res.bestBid = -1.0f;
        __bg_api_GET(close);
        res.priceChangePercent = -1.0f;
        res.chgUtc = -1.0f;
    }

    return res;
}

std::ostream &operator<<(std::ostream &os, const ticker_t &f) {
    os << "{\n"
    << "\t\"symbol\":" << f.symbol << ",\n"
    << "\t\"last\":" << f.last << ",\n"
    << "\t\"bestAsk\":" << f.bestAsk << ",\n"
    << "\t\"bestBid\":" << f.bestBid << ",\n"
    << "\t\"high24h\":" << f.high24h << ",\n"
    << "\t\"low24h\":" << f.low24h << ",\n"
    << "\t\"close\":" << f.close << ",\n"
    << "\t\"quoteVol\":" << f.quoteVol << ",\n"
    << "\t\"baseVol\":" << f.baseVol << ",\n"
    << "\t\"usdtVol\":" << f.usdtVol << ",\n"
    << "\t\"ts\":" << f.ts << ",\n"
    << "\t\"buyOne\":" << f.buyOne << ",\n"
    << "\t\"sellOne\":" << f.sellOne << ",\n"
    << "\t\"bidSz\":" << f.bidSz << ",\n"
    << "\t\"askSz\":" << f.askSz << ",\n"
    << "\t\"openUtc0\":" << f.openUtc0 << ",\n"
    << "\t\"priceChangePercent\":" << f.priceChangePercent << ",\n"
    << "\t\"chgUtc\":" << f.chgUtc << "\n}";

    return os;
}

//------------------------------------------------------------------------------

trade_t trade_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    trade_t res = {};
    __bg_api_GET(symbol);
    __bg_api_GET(tradeId);
    __bg_api_GET(fillPrice);
    __bg_api_GET(fillQuantity);
    res.side = side_from_string(json.at("side").to_string());
    __bg_api_GET(fillTime);

    return res;
}

std::ostream &operator<<(std::ostream &os, const trade_t &f) {
    os << "{\n"
    << "\t\"symbol\":" << f.symbol << ",\n"
    << "\t\"tradeId\":" << f.tradeId << ",\n"
    << "\t\"fillPrice\":" << f.fillPrice << ",\n"
    << "\t\"fillQuantity\":" << f.fillQuantity << ",\n"
    << "\t\"side\":" << side_to_string(f.side) << ",\n"
    << "\t\"fillTime\":" << f.fillTime << "\n}";

    return os;
}

//------------------------------------------------------------------------------

candle_t candle_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    candle_t res = {};
    __bg_api_GET(open);
    __bg_api_GET(high);
    __bg_api_GET(low);
    __bg_api_GET(close);
    __bg_api_GET(baseVol);
    __bg_api_GET(quoteVol);
    __bg_api_GET(ts);

    return res;
}

std::ostream &operator<<(std::ostream &os, const candle_t &f) {
    os << "{\n"
    << "\t\"open\":" << f.open << ",\n"
    << "\t\"high\":" << f.high << ",\n"
    << "\t\"low\":" << f.low << ",\n"
    << "\t\"close\":" << f.close << ",\n"
    << "\t\"baseVol\":" << f.baseVol << ",\n"
    << "\t\"quoteVol\":" << f.quoteVol << ",\n"
    << "\t\"ts\":" << f.ts << "\n}";

    return os;
}

//------------------------------------------------------------------------------

depth_t depth_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    depth_t res = {};
    __bg_api_GET(timestamp);
    const auto asks = json.at("asks");
    const auto bids = json.at("bids");
    assert(asks.is_array());
    assert(bids.is_array());
    assert(asks.size() == bids.size());
    for (auto idx = 0; idx < asks.size(); ++idx) {
        const auto a = asks[idx];
        const auto b = bids[idx];
        assert(a.is_array());
        assert(b.is_array());
        assert(a.size() == 2);
        assert(b.size() == 2);
        double_type a_price = a[0].to_double();
        double_type a_quantity = a[1].to_double();
        double_type b_price = b[0].to_double();
        double_type b_quantity = b[1].to_double();
        res.asks.emplace(std::move(a_price), std::move(a_quantity));
        res.bids.emplace(std::move(b_price), std::move(b_quantity));
    }

    return res;
}

std::ostream &operator<<(std::ostream &os, const depth_t &f) {
    os << "{\n"
    << "\t\"timestamp\":" << f.timestamp << ",\n"
    << "\t\"asks\":[\n";
    for (const auto &a : f.asks) {
        os << "\t\t[" << a.first << ", " << a.second << "],\n";
    }
    os << "\t],\n"
    << "\t\"bids\":[\n";
    for (const auto &b : f.bids) {
        os << "\t\t[" << b.first << ", " << b.second << "],\n";
    }
    os << "\t]\n}";

    return os;
}

//------------------------------------------------------------------------------

contract_t contract_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    contract_t res = {};
    __bg_api_GET(baseCoin);
    __bg_api_GET(quoteCoin);
    __bg_api_GET(symbol);
    __bg_api_GET(buyLimitPriceRatio);
    __bg_api_GET(sellLimitPriceRatio);
    __bg_api_GET(feeRateUpRatio);
    __bg_api_GET(openCostUpRatio);
    __bg_api_GET(sizeMultiplier);
    __bg_api_GET(makerFeeRate);
    __bg_api_GET(takerFeeRate);
    __bg_api_GET(minTradeNum);
    __bg_api_GET(priceEndStep);
    __bg_api_GET(pricePlace);
    __bg_api_GET(volumePlace);
    __bg_api_GET(symbolType);

    const auto arr = json.at("supportMarginCoins");
    assert(arr.is_array());
    for (auto idx = 0; idx < arr.size(); ++idx) {
        assert(arr[idx].is_string());
        std::string coin = arr[idx].to_string();
        res.supportMarginCoins.push_back(std::move(coin));
    }

    return res;
}

std::ostream &operator<<(std::ostream &os, const contract_t &f) {
    os << "{\n"
    << "\t\"baseCoin\":" << f.baseCoin << ",\n"
    << "\t\"quoteCoin\":" << f.quoteCoin << ",\n"
    << "\t\"symbol\":" << f.symbol << ",\n"
    << "\t\"buyLimitPriceRatio\":" << f.buyLimitPriceRatio << ",\n"
    << "\t\"sellLimitPriceRatio\":" << f.sellLimitPriceRatio << ",\n"
    << "\t\"feeRateUpRatio\":" << f.feeRateUpRatio << ",\n"
    << "\t\"openCostUpRatio\":" << f.openCostUpRatio << ",\n"
    << "\t\"sizeMultiplier\":" << f.sizeMultiplier << ",\n"
    << "\t\"makerFeeRate\":" << f.makerFeeRate << ",\n"
    << "\t\"takerFeeRate\":" << f.takerFeeRate << ",\n"
    << "\t\"minTradeNum\":" << f.minTradeNum << ",\n"
    << "\t\"priceEndStep\":" << f.priceEndStep << ",\n"
    << "\t\"pricePlace\":" << f.pricePlace << ",\n"
    << "\t\"volumePlace\":" << f.volumePlace << ",\n"
    << "\t\"symbolType\":" << f.symbolType << ",\n"
    << "\t\"supportMarginCoins\":[\n";
    for (const auto &c : f.supportMarginCoins) {
        os << "\t\t" << c << ",\n";
    }
    os << "\t]\n}";

    return os;
}

//------------------------------------------------------------------------------

fill_t fill_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    fill_t res = {};
    __bg_api_GET(tradeId);
    __bg_api_GET(price);
    __bg_api_GET(size);
    res.side = side_from_string(json.at("side").to_string());
    __bg_api_GET(timestamp);
    __bg_api_GET(symbol);
    
    return res;
}

std::ostream &operator<<(std::ostream &os, const fill_t &f) {
    os << "{\n"
    << "\t\"tradeId\":" << f.tradeId << ",\n"
    << "\t\"price\":" << f.price << ",\n"
    << "\t\"size\":" << f.size << ",\n"
    << "\t\"side\":" << side_to_string(f.side) << ",\n"
    << "\t\"timestamp\":" << f.timestamp << ",\n"
    << "\t\"symbol\":" << f.symbol << "\n}";

    return os;
}

//------------------------------------------------------------------------------

leverage_t leverage_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    leverage_t res = {};
    __bg_api_GET(symbol);
    __bg_api_GET(minLeverage);
    __bg_api_GET(maxLeverage);

    return res;
}

std::ostream &operator<<(std::ostream &os, const leverage_t &f) {
    os << "{\n"
    << "\t\"symbol\":" << f.symbol << ",\n"
    << "\t\"minLeverage\":" << f.minLeverage << ",\n"
    << "\t\"maxLeverage\":" << f.maxLeverage << "\n}";

    return os;
}

//------------------------------------------------------------------------------

address_t address_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    address_t res = {};
    __bg_api_GET(address);
    __bg_api_GET(coin);
    __bg_api_GET(chain);
    __bg_api_GET(tag);
    __bg_api_GET(url);

    return res;
}

std::ostream &operator<<(std::ostream &os, const address_t &f) {
    os << "{\n"
    << "\t\"address\":" << f.address << ",\n"
    << "\t\"coin\":" << f.coin << ",\n"
    << "\t\"chain\":" << f.chain << ",\n"
    << "\t\"tag\":" << f.tag << ",\n"
    << "\t\"url\":" << f.url << "\n}";

    return os;
}

//------------------------------------------------------------------------------

deposit_withdrawal_t deposit_withdrawal_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    deposit_withdrawal_t res = {};
    __bg_api_GET(id);
    __bg_api_GET(txId);
    __bg_api_GET(coin);
    __bg_api_GET(type);
    __bg_api_GET(amount);
    __bg_api_GET(status);
    __bg_api_GET(toAddress);
    __bg_api_GET(chain);
    __bg_api_GET(cTime);
    __bg_api_GET(uTime);
    if (strcmp(res.type.c_str(), "withdrawal") == 0) {
        __bg_api_GET(fee);
        __bg_api_GET(confirm);
    }

    return res;
}

std::ostream &operator<<(std::ostream &os, const deposit_withdrawal_t &f) {
    os << "{\n"
    << "\t\"id\":" << f.id << ",\n"
    << "\t\"txId\":" << f.txId << ",\n"
    << "\t\"coin\":" << f.coin << ",\n"
    << "\t\"type\":" << f.type << ",\n"
    << "\t\"amount\":" << f.amount << ",\n"
    << "\t\"status\":" << f.status << ",\n"
    << "\t\"toAddress\":" << f.toAddress << ",\n"
    << "\t\"fee\":" << f.fee << ",\n"
    << "\t\"chain\":" << f.chain << ",\n"
    << "\t\"confirm\":" << f.confirm << ",\n"
    << "\t\"cTime\":" << f.cTime << ",\n"
    << "\t\"uTime\":" << f.uTime << "\n}";

    return os;
}

//------------------------------------------------------------------------------

apikey_t apikey_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    apikey_t res = {};
    __bg_api_GET(userId);
    __bg_api_GET(inviterId);
    __bg_api_GET(ips);
    __bg_api_GET(auths);
    __bg_api_GET(parentId);
    __bg_api_GET(trader);

    return res;
}

std::ostream &operator<<(std::ostream &os, const apikey_t &f) {
    os << "{\n"
    << "\t\"userId\":" << f.userId << ",\n"
    << "\t\"inviterId\":" << f.inviterId << ",\n"
    << "\t\"ips\":" << f.ips << ",\n"
    << "\t\"auths\":" << f.auths << ",\n"
    << "\t\"parentId\":" << f.parentId << ",\n"
    << "\t\"trader\":" << f.trader << "\n}";

    return os;
}

//------------------------------------------------------------------------------

spot_account_t spot_account_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    spot_account_t res = {};
    __bg_api_GET(coinId);
    __bg_api_GET(coinName);
    __bg_api_GET(available);
    __bg_api_GET(lock);
    __bg_api_GET(uTime);

    return res;
}

std::ostream &operator<<(std::ostream &os, const spot_account_t &f) {
    os << "{\n"
    << "\t\"coinId\":" << f.coinId << ",\n"
    << "\t\"coinName\":" << f.coinName << ",\n"
    << "\t\"available\":" << f.available << ",\n"
    << "\t\"lock\":" << f.lock << ",\n"
    << "\t\"uTime\":" << f.uTime << "\n}";

    return os;
}

//------------------------------------------------------------------------------

bill_t bill_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    bill_t res = {};
    __bg_api_GET(coinId);
    __bg_api_GET(coinName);
    res.groupType = group_type_from_string(json["groupType"].to_string());
    res.bizType = biz_type_from_string(json["bizType"].to_string());
    __bg_api_GET(quantity);
    __bg_api_GET(balance);
    __bg_api_GET(fees);
    __bg_api_GET(billId);

    return res;
}

std::ostream &operator<<(std::ostream &os, const bill_t &f) {
    os << "{\n"
    << "\t\"coinId\":" << f.coinId << ",\n"
    << "\t\"coinName\":" << f.coinName << ",\n"
    << "\t\"groupType\":" << group_type_to_string(f.groupType) << ",\n"
    << "\t\"bizType\":" << biz_type_to_string(f.bizType) << ",\n"
    << "\t\"quantity\":" << f.quantity << ",\n"
    << "\t\"balance\":" << f.balance << ",\n"
    << "\t\"fees\":" << f.fees << ",\n"
    << "\t\"billId\":" << f.billId << "\n}";

    return os;
}

//------------------------------------------------------------------------------

transfer_t transfer_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    transfer_t res = {};
    __bg_api_GET(coinName);
    __bg_api_GET(status);
    __bg_api_GET(toType);
    __bg_api_GET(toSymbol);
    __bg_api_GET(fromType);
    __bg_api_GET(fromSymbol);
    __bg_api_GET(amount);
    __bg_api_GET(tradeTime);

    return res;
}

std::ostream &operator<<(std::ostream &os, const transfer_t &f) {
    os << "{\n"
    << "\t\"coinName\":" << f.coinName << ",\n"
    << "\t\"status\":" << f.status << ",\n"
    << "\t\"toType\":" << f.toType << ",\n"
    << "\t\"toSymbol\":" << f.toSymbol << ",\n"
    << "\t\"fromType\":" << f.fromType << ",\n"
    << "\t\"fromSymbol\":" << f.fromSymbol << ",\n"
    << "\t\"amount\":" << f.amount << ",\n"
    << "\t\"tradeTime\":" << f.tradeTime << "\n}";

    return os;
}

//------------------------------------------------------------------------------

futures_account_t futures_account_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    futures_account_t res = {};
    __bg_api_GET(marginCoin);
    __bg_api_GET(locked);
    __bg_api_GET(available);
    __bg_api_GET(crossMaxAvailable);
    __bg_api_GET(fixedMaxAvailable);
    __bg_api_GET(maxTransferOut);
    __bg_api_GET(equity);
    __bg_api_GET(usdtEquity);
    __bg_api_GET(btcEquity);
    __bg_api_GET(crossRiskRate);
    __bg_api_GET(crossMarginLeverage);
    __bg_api_GET(fixedLongLeverage);
    __bg_api_GET(fixedShortLeverage);
    res.marginMode = margin_mode_from_string(json["marginMode"].to_string());
    res.holdMode = hold_mode_from_string(json["holdMode"].to_string());

    return res;
}

std::ostream &operator<<(std::ostream &os, const futures_account_t &f) {
    os << "{\n"
    << "\t\"marginCoin\":" << f.marginCoin << ",\n"
    << "\t\"locked\":" << f.locked << ",\n"
    << "\t\"available\":" << f.available << ",\n"
    << "\t\"crossMaxAvailable\":" << f.crossMaxAvailable << ",\n"
    << "\t\"fixedMaxAvailable\":" << f.fixedMaxAvailable << ",\n"
    << "\t\"maxTransferOut\":" << f.maxTransferOut << ",\n"
    << "\t\"equity\":" << f.equity << ",\n"
    << "\t\"usdtEquity\":" << f.usdtEquity << ",\n"
    << "\t\"btcEquity\":" << f.btcEquity << ",\n"
    << "\t\"crossRiskRate\":" << f.crossRiskRate << ",\n"
    << "\t\"crossMarginLeverage\":" << f.crossMarginLeverage << ",\n"
    << "\t\"fixedLongLeverage\":" << f.fixedLongLeverage << ",\n"
    << "\t\"fixedShortLeverage\":" << f.fixedShortLeverage << ",\n"
    << "\t\"marginMode\":" << margin_mode_to_string(f.marginMode) << ",\n"
    << "\t\"holdMode\":" << hold_mode_to_string(f.holdMode) << "\n}";

    return os;
}

//------------------------------------------------------------------------------

position_t position_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    position_t res = {};
    __bg_api_GET(marginCoin);
    __bg_api_GET(symbol);
    res.holdSide = hold_side_from_string(json["holdSide"].to_string());
    __bg_api_GET(openDelegateCount);
    __bg_api_GET(margin);
    __bg_api_GET(available);
    __bg_api_GET(locked);
    __bg_api_GET(total);
    __bg_api_GET(leverage);
    __bg_api_GET(achievedProfits);
    __bg_api_GET(averageOpenPrice);
    res.marginMode = margin_mode_from_string(json["marginMode"].to_string());
    res.holdMode = hold_mode_from_string(json["holdMode"].to_string());
    __bg_api_GET(unrealizedPL);
    __bg_api_GET(keepMarginRate);
    __bg_api_GET(marketPrice);
    __bg_api_GET(cTime);

    return res;
}

std::ostream &operator<<(std::ostream &os, const position_t &f) {
    os << "{\n"
    << "\t\"marginCoin\":" << f.marginCoin << ",\n"
    << "\t\"symbol\":" << f.symbol << ",\n"
    << "\t\"holdSide\":" << hold_side_to_string(f.holdSide) << ",\n"
    << "\t\"openDelegateCount\":" << f.openDelegateCount << ",\n"
    << "\t\"margin\":" << f.margin << ",\n"
    << "\t\"available\":" << f.available << ",\n"
    << "\t\"locked\":" << f.locked << ",\n"
    << "\t\"total\":" << f.total << ",\n"
    << "\t\"leverage\":" << f.leverage << ",\n"
    << "\t\"achievedProfits\":" << f.achievedProfits << ",\n"
    << "\t\"averageOpenPrice\":" << f.averageOpenPrice << ",\n"
    << "\t\"marginMode\":" << margin_mode_to_string(f.marginMode) << ",\n"
    << "\t\"holdMode\":" << hold_mode_to_string(f.holdMode) << ",\n"
    << "\t\"unrealizedPL\":" << f.unrealizedPL << ",\n"
    << "\t\"keepMarginRate\":" << f.keepMarginRate << ",\n"
    << "\t\"marketPrice\":" << f.marketPrice << ",\n"
    << "\t\"cTime\":" << f.cTime << "\n}";

    return os;
}

//------------------------------------------------------------------------------

futures_bill_t futures_bill_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    futures_bill_t res = {};
    __bg_api_GET(id);
    __bg_api_GET(symbol);
    __bg_api_GET(marginCoin);
    __bg_api_GET(amount);
    __bg_api_GET(fee);
    __bg_api_GET(feeByCoupon);
    __bg_api_GET(feeCoin);
    res.business = business_from_string(json["business"].to_string());
    __bg_api_GET(cTime);
    
    return res;
}

std::ostream &operator<<(std::ostream &os, const futures_bill_t &f) {
    os << "{\n"
    << "\t\"id\":" << f.id << ",\n"
    << "\t\"symbol\":" << f.symbol << ",\n"
    << "\t\"marginCoin\":" << f.marginCoin << ",\n"
    << "\t\"amount\":" << f.amount << ",\n"
    << "\t\"fee\":" << f.fee << ",\n"
    << "\t\"feeByCoupon\":" << f.feeByCoupon << ",\n"
    << "\t\"feeCoin\":" << f.feeCoin << ",\n"
    << "\t\"business\":" << business_to_string(f.business) << ",\n"
    << "\t\"cTime\":" << f.cTime << "\n}";

    return os;
}

//------------------------------------------------------------------------------

coin_leverage_t coin_leverage_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    coin_leverage_t res = {};
    __bg_api_GET(symbol);
    __bg_api_GET(marginCoin);
    __bg_api_GET(longLeverage);
    __bg_api_GET(shortLeverage);
    res.marginMode = margin_mode_from_string(json["marginMode"].to_string());

    return res;
}

std::ostream &operator<<(std::ostream &os, const coin_leverage_t &f) {
    os << "{\n"
    << "\t\"symbol\":" << f.symbol << ",\n"
    << "\t\"marginCoin\":" << f.marginCoin << ",\n"
    << "\t\"longLeverage\":" << f.longLeverage << ",\n"
    << "\t\"shortLeverage\":" << f.shortLeverage << ",\n"
    << "\t\"marginMode\":" << margin_mode_to_string(f.marginMode) << "\n}";

    return os;
}

//------------------------------------------------------------------------------

place_spot_order_t place_spot_order_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    place_spot_order_t res = {};
    res.side = side_from_string(json["side"].to_string());
    res.orderType = order_type_from_string(json["orderType"].to_string());
    res.force = force_from_string(json["force"].to_string());
    __bg_api_GET(price);
    __bg_api_GET(quantity);
    __bg_api_GET(clientOrderId);

    return res;
}

std::ostream &operator<<(std::ostream &os, const place_spot_order_t &f) {
    os << "{\n"
    << "\t\"side\":" << side_to_string(f.side) << ",\n"
    << "\t\"orderType\":" << order_type_to_string(f.orderType) << ",\n"
    << "\t\"force\":" << force_to_string(f.force) << ",\n"
    << "\t\"price\":" << f.price << ",\n"
    << "\t\"quantity\":" << f.quantity << ",\n"
    << "\t\"clientOrderId\":" << f.clientOrderId << "\n}";

    return os;
}

//------------------------------------------------------------------------------

place_futures_order_t place_futures_order_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    place_futures_order_t res = {};
    if (json.contains("reverse")) {
        __bg_api_GET(reverse);
    } else {
        res.reverse = false;
    }
    __bg_api_GET(size);
    res.side = mix_side_from_string(json["side"].to_string());
    res.orderType = order_type_from_string(json["orderType"].to_string());
    res.force = force_from_string(json["force"].to_string());
    __bg_api_GET(clientOid);
    if (!res.reverse) {
        __bg_api_GET(price);
        __bg_api_GET(presetTP);
        __bg_api_GET(presetSL);
    } else {
        res.price = -1.0f;
        res.presetTP = -1.0f;
        res.presetSL = -1.0f;
    }

    return res;
}

std::ostream &operator<<(std::ostream &os, const place_futures_order_t &f) {
    os << "{\n"
    << "\t\"reverse\":" << f.reverse << ",\n"
    << "\t\"size\":" << f.size << ",\n"
    << "\t\"side\":" << mix_side_to_string(f.side) << ",\n"
    << "\t\"orderType\":" << order_type_to_string(f.orderType) << ",\n"
    << "\t\"force\":" << force_to_string(f.force) << ",\n"
    << "\t\"clientOid\":" << f.clientOid << ",\n"
    << "\t\"price\":" << f.price << ",\n"
    << "\t\"presetTP\":" << f.presetTP << ",\n"
    << "\t\"presetSL\":" << f.presetSL << "\n}";

    return os;
}

//------------------------------------------------------------------------------

spot_order_t spot_order_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    spot_order_t res = {};
    __bg_api_GET(accountId);
    __bg_api_GET(symbol);
    __bg_api_GET(orderId);
    __bg_api_GET(clientOrderId);
    __bg_api_GET(price);
    __bg_api_GET(quantity);
    res.orderType = order_type_from_string(json["orderType"].to_string());
    res.side = side_from_string(json["side"].to_string());
    res.status = status_from_string(json["status"].to_string());
    __bg_api_GET(fillPrice);
    __bg_api_GET(fillQuantity);
    __bg_api_GET(fillTotalAmount);
    __bg_api_GET(cTime);

    return res;
}

std::ostream &operator<<(std::ostream &os, const spot_order_t &f) {
    os << "{\n"
    << "\t\"accountId\":" << f.accountId << ",\n"
    << "\t\"symbol\":" << f.symbol << ",\n"
    << "\t\"orderId\":" << f.orderId << ",\n"
    << "\t\"clientOrderId\":" << f.clientOrderId << ",\n"
    << "\t\"price\":" << f.price << ",\n"
    << "\t\"quantity\":" << f.quantity << ",\n"
    << "\t\"orderType\":" << order_type_to_string(f.orderType) << ",\n"
    << "\t\"side\":" << side_to_string(f.side) << ",\n"
    << "\t\"status\":" << status_to_string(f.status) << ",\n"
    << "\t\"fillPrice\":" << f.fillPrice << ",\n"
    << "\t\"fillQuantity\":" << f.fillQuantity << ",\n"
    << "\t\"fillTotalAmount\":" << f.fillTotalAmount << ",\n"
    << "\t\"cTime\":" << f.cTime << "\n}";

    return os;
}

//------------------------------------------------------------------------------

futures_order_t futures_order_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    futures_order_t res = {};
    __bg_api_GET(symbol);
    __bg_api_GET(size);
    __bg_api_GET(orderId);
    __bg_api_GET(clientOid);
    __bg_api_GET(filledQty);
    __bg_api_GET(fee);
    __bg_api_GET(price);
    if (json.contains("priceAvg")) {
        __bg_api_GET(priceAvg);
    } else {
        res.priceAvg = -1.0f;
    }
    res.state = status_from_string(json["status"].to_string());
    res.side = side_from_string(json["side"].to_string());
    res.timeInForce = force_from_string(json["timeInForce"].to_string());
    __bg_api_GET(totalProfits);
    res.posSide = hold_side_from_string(json["posSide"].to_string());
    __bg_api_GET(marginCoin);
    if (json.contains("presetTakeProfitPrice")) {
        __bg_api_GET(presetTakeProfitPrice);
    } else {
        res.presetTakeProfitPrice = -1.0f;
    }
    if (json.contains("presetStopLossPrice")) {
        __bg_api_GET(presetStopLossPrice);
    } else {
        res.presetStopLossPrice = -1.0f;
    }
    if (json.contains("filledAmount")) {
        __bg_api_GET(filledAmount);
    } else {
        res.filledAmount = -1.0f;
    }
    if (json.contains("leverage")) {
        __bg_api_GET(leverage);
    } else {
        res.leverage = 1;
    }
    res.orderType = order_type_from_string(json["orderType"].to_string());
    if (json.contains("marginMode")) {
        res.marginMode = margin_mode_from_string(json["marginMode"].to_string());
    } else {
        res.marginMode = _margin_mode::_crossed;
    }
    __bg_api_GET(cTime);
    if (json.contains("uTime")) {
        __bg_api_GET(uTime);
    } else {
        res.uTime = 0;
    }

    return res;
}

std::ostream &operator<<(std::ostream &os, const futures_order_t &f) {
    os << "{\n"
    << "\t\"symbol\":" << f.symbol << ",\n"
    << "\t\"size\":" << f.size << ",\n"
    << "\t\"orderId\":" << f.orderId << ",\n"
    << "\t\"clientOid\":" << f.clientOid << ",\n"
    << "\t\"filledQty\":" << f.filledQty << ",\n"
    << "\t\"fee\":" << f.fee << ",\n"
    << "\t\"price\":" << f.price << ",\n"
    << "\t\"priceAvg\":" << f.priceAvg << ",\n"
    << "\t\"state\":" << status_to_string(f.state) << ",\n"
    << "\t\"side\":" << side_to_string(f.side) << ",\n"
    << "\t\"timeInForce\":" << force_to_string(f.timeInForce) << ",\n"
    << "\t\"totalProfits\":" << f.totalProfits << ",\n"
    << "\t\"posSide\":" << hold_side_to_string(f.posSide) << ",\n"
    << "\t\"marginCoin\":" << f.marginCoin << ",\n"
    << "\t\"presetTakeProfitPrice\":" << f.presetTakeProfitPrice << ",\n"
    << "\t\"presetStopLossPrice\":" << f.presetStopLossPrice << ",\n"
    << "\t\"filledAmount\":" << f.filledAmount << ",\n"
    << "\t\"leverage\":" << f.leverage << ",\n"
    << "\t\"orderType\":" << order_type_to_string(f.orderType) << ",\n"
    << "\t\"marginMode\":" << margin_mode_to_string(f.marginMode) << ",\n"
    << "\t\"cTime\":" << f.cTime << ",\n"
    << "\t\"uTime\":" << f.uTime << "\n}";

    return os;
}

//------------------------------------------------------------------------------

transaction_t transaction_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    transaction_t res = {};
    __bg_api_GET(accountId);
    __bg_api_GET(symbol);
    __bg_api_GET(orderId);
    __bg_api_GET(fillId);
    res.orderType = order_type_from_string(json["orderType"].to_string());
    res.side = side_from_string(json["side"].to_string());
    __bg_api_GET(fillPrice);
    __bg_api_GET(fillQuantity);
    __bg_api_GET(fillTotalAmount);
    __bg_api_GET(cTime);
    __bg_api_GET(feeCcy);
    __bg_api_GET(fees);

    return res;
}

std::ostream &operator<<(std::ostream &os, const transaction_t &f) {
    os << "{\n"
    << "\t\"accountId\":" << f.accountId << ",\n"
    << "\t\"symbol\":" << f.symbol << ",\n"
    << "\t\"orderId\":" << f.orderId << ",\n"
    << "\t\"fillId\":" << f.fillId << ",\n"
    << "\t\"orderType\":" << order_type_to_string(f.orderType) << ",\n"
    << "\t\"side\":" << side_to_string(f.side) << ",\n"
    << "\t\"fillPrice\":" << f.fillPrice << ",\n"
    << "\t\"fillQuantity\":" << f.fillQuantity << ",\n"
    << "\t\"fillTotalAmount\":" << f.fillTotalAmount << ",\n"
    << "\t\"cTime\":" << f.cTime << ",\n"
    << "\t\"feeCcy\":" << f.feeCcy << ",\n"
    << "\t\"fees\":" << f.fees << "\n}";

    return os;
}

//------------------------------------------------------------------------------

place_plan_order_spot_t place_plan_order_spot_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    place_plan_order_spot_t res = {};
    res.side = side_from_string(json["side"].to_string());
    __bg_api_GET(triggerPrice);
    __bg_api_GET(executePrice);
    __bg_api_GET(size);
    res.triggerType = trigger_type_from_string(json["triggerType"].to_string());
    res.orderType = order_type_from_string(json["orderType"].to_string());
    __bg_api_GET(clientOid);
    res.force = force_from_string(json["timeInForce"].to_string());

    return res;
}

std::ostream &operator<<(std::ostream &os, const place_plan_order_spot_t &f) {
    os << "{\n"
    << "\t\"side\":" << side_to_string(f.side) << ",\n"
    << "\t\"triggerPrice\":" << f.triggerPrice << ",\n"
    << "\t\"executePrice\":" << f.executePrice << ",\n"
    << "\t\"size\":" << f.size << ",\n"
    << "\t\"triggerType\":" << trigger_type_to_string(f.triggerType) << ",\n"
    << "\t\"orderType\":" << order_type_to_string(f.orderType) << ",\n"
    << "\t\"clientOid\":" << f.clientOid << ",\n"
    << "\t\"timeInForce\":" << force_to_string(f.force) << "\n}";

    return os;
}

//------------------------------------------------------------------------------

place_plan_order_futures_t place_plan_order_futures_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    place_plan_order_futures_t res = {};
    __bg_api_GET(size);
    __bg_api_GET(executePrice);
    __bg_api_GET(triggerPrice);
    res.side = mix_side_from_string(json["side"].to_string());
    res.orderType = order_type_from_string(json["orderType"].to_string());
    res.force = force_from_string(json["timeInForce"].to_string());
    res.triggerType = trigger_type_from_string(json["triggerType"].to_string());
    __bg_api_GET(clientOid);
    __bg_api_GET(presetSL);
    __bg_api_GET(presetTP);

    return res;
}

std::ostream &operator<<(std::ostream &os, const place_plan_order_futures_t &f) {
    os << "{\n"
    << "\t\"size\":" << f.size << ",\n"
    << "\t\"executePrice\":" << f.executePrice << ",\n"
    << "\t\"triggerPrice\":" << f.triggerPrice << ",\n"
    << "\t\"side\":" << mix_side_to_string(f.side) << ",\n"
    << "\t\"orderType\":" << order_type_to_string(f.orderType) << ",\n"
    << "\t\"timeInForce\":" << force_to_string(f.force) << ",\n"
    << "\t\"triggerType\":" << trigger_type_to_string(f.triggerType) << ",\n"
    << "\t\"clientOid\":" << f.clientOid << ",\n"
    << "\t\"presetSL\":" << f.presetSL << ",\n"
    << "\t\"presetTP\":" << f.presetTP << "\n}";

    return os;
}

//------------------------------------------------------------------------------

spot_plan_order_t spot_plan_order_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    spot_plan_order_t res = {};
    __bg_api_GET(orderId);
    if (json.contains("executeOrderId")) {
        __bg_api_GET(executeOrderId);
    } else {
        res.executeOrderId = "";
    }
    __bg_api_GET(symbol);
    if (json.contains("marginCoin")) {
        __bg_api_GET(marginCoin);
        __bg_api_GET(presetTakeProfitPrice);
        __bg_api_GET(presetStopLossPrice);
    } else {
        res.marginCoin = "";
        res.presetTakeProfitPrice = -1.0f;
        res.presetStopLossPrice = -1.0f;
    }
    __bg_api_GET(size);
    __bg_api_GET(executePrice);
    __bg_api_GET(triggerPrice);
    res.status = plan_status_from_string(json["status"].to_string());
    res.orderType = order_type_from_string(json["orderType"].to_string());
    res.side = side_from_string(json["side"].to_string());
    res.triggerType = trigger_type_from_string(json["triggerType"].to_string());
    __bg_api_GET(cTime);

    return res;
}

std::ostream &operator<<(std::ostream &os, const spot_plan_order_t &f) {
    os << "{\n"
    << "\t\"orderId\":" << f.orderId << ",\n"
    << "\t\"executeOrderId\":" << f.executeOrderId << ",\n"
    << "\t\"symbol\":" << f.symbol << ",\n"
    << "\t\"marginCoin\":" << f.marginCoin << ",\n"
    << "\t\"presetTakeProfitPrice\":" << f.presetTakeProfitPrice << ",\n"
    << "\t\"presetStopLossPrice\":" << f.presetStopLossPrice << ",\n"
    << "\t\"size\":" << f.size << ",\n"
    << "\t\"executePrice\":" << f.executePrice << ",\n"
    << "\t\"triggerPrice\":" << f.triggerPrice << ",\n"
    << "\t\"status\":" << plan_status_to_string(f.status) << ",\n"
    << "\t\"orderType\":" << order_type_to_string(f.orderType) << ",\n"
    << "\t\"side\":" << side_to_string(f.side) << ",\n"
    << "\t\"triggerType\":" << trigger_type_to_string(f.triggerType) << ",\n"
    << "\t\"cTime\":" << f.cTime << "\n}";

    return os;
}

//------------------------------------------------------------------------------

futures_plan_order_t futures_plan_order_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    futures_plan_order_t res = {};
    __bg_api_GET(orderId);
    if (json.contains("executeOrderId")) {
        __bg_api_GET(executeOrderId);
    } else {
        res.executeOrderId = "";
    }
    __bg_api_GET(symbol);
    __bg_api_GET(marginCoin);
    __bg_api_GET(size);
    __bg_api_GET(executePrice);
    __bg_api_GET(triggerPrice);
    res.status = plan_status_from_string(json["status"].to_string());
    res.orderType = order_type_from_string(json["orderType"].to_string());
    res.side = mix_side_from_string(json["side"].to_string());
    res.triggerType = trigger_type_from_string(json["triggerType"].to_string());
    __bg_api_GET(presetTakeProfitPrice);
    __bg_api_GET(presetStopLossPrice);

    return res;
}

std::ostream &operator<<(std::ostream &os, const futures_plan_order_t &f) {
    os << "{\n"
    << "\t\"orderId\":" << f.orderId << ",\n"
    << "\t\"executeOrderId\":" << f.executeOrderId << ",\n"
    << "\t\"symbol\":" << f.symbol << ",\n"
    << "\t\"marginCoin\":" << f.marginCoin << ",\n"
    << "\t\"size\":" << f.size << ",\n"
    << "\t\"executePrice\":" << f.executePrice << ",\n"
    << "\t\"triggerPrice\":" << f.triggerPrice << ",\n"
    << "\t\"status\":" << plan_status_to_string(f.status) << ",\n"
    << "\t\"orderType\":" << order_type_to_string(f.orderType) << ",\n"
    << "\t\"side\":" << mix_side_to_string(f.side) << ",\n"
    << "\t\"triggerType\":" << trigger_type_to_string(f.triggerType) << ",\n"
    << "\t\"presetTakeProfitPrice\":" << f.presetTakeProfitPrice << ",\n"
    << "\t\"presetStopLossPrice\":" << f.presetStopLossPrice << "\n}";

    return os;
}

//------------------------------------------------------------------------------

futures_fill_t futures_fill_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    futures_fill_t res = {};
    __bg_api_GET(tradeId);
    __bg_api_GET(symbol);
    __bg_api_GET(orderId);
    __bg_api_GET(price);
    __bg_api_GET(sizeQty);
    __bg_api_GET(fee);
    res.side = trade_side_from_string(json["side"].to_string());
    __bg_api_GET(cTime);

    return res;
}

std::ostream &operator<<(std::ostream &os, const futures_fill_t &f) {
    os << "{\n"
    << "\t\"tradeId\":" << f.tradeId << ",\n"
    << "\t\"symbol\":" << f.symbol << ",\n"
    << "\t\"orderId\":" << f.orderId << ",\n"
    << "\t\"price\":" << f.price << ",\n"
    << "\t\"sizeQty\":" << f.sizeQty << ",\n"
    << "\t\"fee\":" << f.fee << ",\n"
    << "\t\"side\":" << trade_side_to_string(f.side) << ",\n"
    << "\t\"cTime\":" << f.cTime << "\n}";

    return os;
}

//------------------------------------------------------------------------------

trader_order_t trader_order_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    trader_order_t res = {};
    __bg_api_GET(symbol);
    __bg_api_GET(trackingNo);
    __bg_api_GET(openOrderId);
    res.holdSide = hold_side_from_string(json["holdSide"].to_string());
    __bg_api_GET(openLeverage);
    __bg_api_GET(openAvgPrice);
    __bg_api_GET(openTime);
    __bg_api_GET(openDealCount);
    __bg_api_GET(stopProfitPrice);
    __bg_api_GET(stopLossPrice);
    __bg_api_GET(closeDealCount);
    if (json.contains("closeTime")) {
        __bg_api_GET(closeTime);
        __bg_api_GET(closeAvgPrice);
        res.stopType = stop_type_from_string(json["stopType"].to_string());
        __bg_api_GET(achievedProfits);
        __bg_api_GET(openFee);
        __bg_api_GET(closeFee);
    } else {
        res.closeTime = 0;
        res.closeAvgPrice = -1.0f;
        res.stopType = _stop_type::_profit;
        res.achievedProfits = -1.0f;
        res.openFee = -1.0f;
        res.closeFee = -1.0f;
    }

    return res;
}

std::ostream &operator<<(std::ostream &os, const trader_order_t &f) {
    os << "{\n"
    << "\t\"symbol\":" << f.symbol << ",\n"
    << "\t\"trackingNo\":" << f.trackingNo << ",\n"
    << "\t\"openOrderId\":" << f.openOrderId << ",\n"
    << "\t\"holdSide\":" << hold_side_to_string(f.holdSide) << ",\n"
    << "\t\"openLeverage\":" << f.openLeverage << ",\n"
    << "\t\"openAvgPrice\":" << f.openAvgPrice << ",\n"
    << "\t\"openTime\":" << f.openTime << ",\n"
    << "\t\"openDealCount\":" << f.openDealCount << ",\n"
    << "\t\"stopProfitPrice\":" << f.stopProfitPrice << ",\n"
    << "\t\"stopLossPrice\":" << f.stopLossPrice << ",\n"
    << "\t\"closeDealCount\":" << f.closeDealCount << ",\n"
    << "\t\"closeTime\":" << f.closeTime << ",\n"
    << "\t\"closeAvgPrice\":" << f.closeAvgPrice << ",\n"
    << "\t\"stopType\":" << stop_type_to_string(f.stopType) << ",\n"
    << "\t\"achievedProfits\":" << f.achievedProfits << ",\n"
    << "\t\"openFee\":" << f.openFee << ",\n"
    << "\t\"closeFee\":" << f.closeFee << "\n}";

    return os;
}

//------------------------------------------------------------------------------

follower_order_t follower_order_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    follower_order_t res = {};
    __bg_api_GET(symbol);
    __bg_api_GET(trackingNo);
    __bg_api_GET(openOrderId);
    __bg_api_GET(closeOrderId);
    res.holdSide = hold_side_from_string(json["holdSide"].to_string());
    __bg_api_GET(openLeverage);
    __bg_api_GET(openAvgPrice);
    __bg_api_GET(openTime);
    __bg_api_GET(openDealCount);
    __bg_api_GET(openMargin);
    __bg_api_GET(avgClosePrice);
    __bg_api_GET(closeDealCount);
    __bg_api_GET(closeTime);
    if (json.contains("openSize")) {
        __bg_api_GET(openSize);
        __bg_api_GET(closeSize);
        res.stopType = stop_type_from_string(json["stopType"].to_string());
        __bg_api_GET(achievedProfits);
        __bg_api_GET(openFee);
        __bg_api_GET(closeFee);
        __bg_api_GET(profitRate);
        __bg_api_GET(netProfit);
    } else {
        res.openSize = -1.0f;
        res.closeSize = -1.0f;
        res.stopType = _stop_type::_profit;
        res.achievedProfits = -1.0f;
        res.openFee = -1.0f;
        res.closeFee = -1.0f;
        res.profitRate = -1.0f;
        res.netProfit = -1.0f;
    }

    return res;
}

std::ostream &operator<<(std::ostream &os, const follower_order_t &f) {
    os << "{\n"
    << "\t\"symbol\":" << f.symbol << ",\n"
    << "\t\"trackingNo\":" << f.trackingNo << ",\n"
    << "\t\"openOrderId\":" << f.openOrderId << ",\n"
    << "\t\"closeOrderId\":" << f.closeOrderId << ",\n"
    << "\t\"holdSide\":" << hold_side_to_string(f.holdSide) << ",\n"
    << "\t\"openLeverage\":" << f.openLeverage << ",\n"
    << "\t\"openAvgPrice\":" << f.openAvgPrice << ",\n"
    << "\t\"openTime\":" << f.openTime << ",\n"
    << "\t\"openDealCount\":" << f.openDealCount << ",\n"
    << "\t\"openMargin\":" << f.openMargin << ",\n"
    << "\t\"avgClosePrice\":" << f.avgClosePrice << ",\n"
    << "\t\"closeDealCount\":" << f.closeDealCount << ",\n"
    << "\t\"closeTime\":" << f.closeTime << ",\n"
    << "\t\"openSize\":" << f.openSize << ",\n"
    << "\t\"closeSize\":" << f.closeSize << ",\n"
    << "\t\"stopType\":" << stop_type_to_string(f.stopType) << ",\n"
    << "\t\"achievedProfits\":" << f.achievedProfits << ",\n"
    << "\t\"openFee\":" << f.openFee << ",\n"
    << "\t\"closeFee\":" << f.closeFee << ",\n"
    << "\t\"profitRate\":" << f.profitRate << ",\n"
    << "\t\"netProfit\":" << f.netProfit << "\n}";

    return os;
}

//------------------------------------------------------------------------------

trader_profit_t trader_profit_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    trader_profit_t res = {};
    __bg_api_GET(yesterdaySplitProfit);
    __bg_api_GET(sumProfit);
    __bg_api_GET(waitProfit);
    __bg_api_GET(yesterdayTimestamp);

    return res;
}

std::ostream &operator<<(std::ostream &os, const trader_profit_t &f) {
    os << "{\n"
    << "\t\"yesterdaySplitProfit\":" << f.yesterdaySplitProfit << ",\n"
    << "\t\"sumProfit\":" << f.sumProfit << ",\n"
    << "\t\"waitProfit\":" << f.waitProfit << ",\n"
    << "\t\"yesterdayTimestamp\":" << f.yesterdayTimestamp << "\n}";

    return os;
}

//------------------------------------------------------------------------------

profit_token_t profit_token_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    profit_token_t res = {};
    __bg_api_GET(tokenId);
    __bg_api_GET(profit);
    if (json.contains("date")) {
        __bg_api_GET(date);
    } else {
        res.date = 0;
    }
    if (json.contains("nickname")) {
        __bg_api_GET(nickname);
    } else {
        res.nickname = "";
    }

    return res;
}

std::ostream &operator<<(std::ostream &os, const profit_token_t &f) {
    os << "{\n"
    << "\t\"tokenId\":" << f.tokenId << ",\n"
    << "\t\"profit\":" << f.profit << ",\n"
    << "\t\"date\":" << f.date << ",\n"
    << "\t\"nickname\":" << f.nickname << "\n}";

    return os;
}

//------------------------------------------------------------------------------

trader_symbols_t trader_symbols_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    trader_symbols_t res = {};
    __bg_api_GET(symbol);
    __bg_api_GET(minOpenCount);
    __bg_api_GET(openTrader);
    
    return res;
}

std::ostream &operator<<(std::ostream &os, const trader_symbols_t &f) {
    os << "{\n"
    << "\t\"symbol\":" << f.symbol << ",\n"
    << "\t\"minOpenCount\":" << f.minOpenCount << ",\n"
    << "\t\"openTrader\":" << f.openTrader << "\n}";

    return os;
}

//------------------------------------------------------------------------------

subaccount_t subaccount_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    subaccount_t res = {};
    __bg_api_GET(subUid);
    __bg_api_GET(subName);
    res.status = account_status_from_string(json["status"].to_string());
    const auto arr = json.at("auths");
    assert(arr.is_array());
    for (auto idx = 0; idx < arr.size(); ++idx) {
        assert(arr[idx].is_string());
        std::string auth = arr[idx].to_string();
        res.auths.emplace(std::move(auth));
    }
    __bg_api_GET(remark);
    __bg_api_GET(cTime);

    return res;
}

std::ostream &operator<<(std::ostream &os, const subaccount_t &f) {
    os << "{\n"
    << "\t\"subUid\":" << f.subUid << ",\n"
    << "\t\"subName\":" << f.subName << ",\n"
    << "\t\"status\":" << account_status_to_string(f.status) << ",\n"
    << "\t\"auths\":[";
    for (auto &auth : f.auths) {
        os << auth << ",";
    }
    os << "],\n"
    << "\t\"remark\":" << f.remark << ",\n"
    << "\t\"cTime\":" << f.cTime << "\n}";

    return os;
}

//------------------------------------------------------------------------------

sub_spot_asset_t sub_spot_asset_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    sub_spot_asset_t res = {};
    __bg_api_GET(coinId);
    __bg_api_GET(coinName);
    __bg_api_GET(available);
    __bg_api_GET(frozen);
    __bg_api_GET(lock);
    __bg_api_GET(uTime);

    return res;
}

std::ostream &operator<<(std::ostream &os, const sub_spot_asset_t &f) {
    os << "{\n"
    << "\t\"coinId\":" << f.coinId << ",\n"
    << "\t\"coinName\":" << f.coinName << ",\n"
    << "\t\"available\":" << f.available << ",\n"
    << "\t\"frozen\":" << f.frozen << ",\n"
    << "\t\"lock\":" << f.lock << ",\n"
    << "\t\"uTime\":" << f.uTime << "\n}";

    return os;
}

//------------------------------------------------------------------------------

sub_futures_asset_t sub_futures_asset_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    sub_futures_asset_t res = {};
    __bg_api_GET(marginCoin);
    __bg_api_GET(locked);
    __bg_api_GET(available);
    __bg_api_GET(crossMaxAvailable);
    __bg_api_GET(fixedMaxAvailable);
    __bg_api_GET(maxTransferOut);
    __bg_api_GET(equity);
    __bg_api_GET(usdtEquity);
    __bg_api_GET(btcEquity);

    return res;
}

std::ostream &operator<<(std::ostream &os, const sub_futures_asset_t &f) {
    os << "{\n"
    << "\t\"marginCoin\":" << f.marginCoin << ",\n"
    << "\t\"locked\":" << f.locked << ",\n"
    << "\t\"available\":" << f.available << ",\n"
    << "\t\"crossMaxAvailable\":" << f.crossMaxAvailable << ",\n"
    << "\t\"fixedMaxAvailable\":" << f.fixedMaxAvailable << ",\n"
    << "\t\"maxTransferOut\":" << f.maxTransferOut << ",\n"
    << "\t\"equity\":" << f.equity << ",\n"
    << "\t\"usdtEquity\":" << f.usdtEquity << ",\n"
    << "\t\"btcEquity\":" << f.btcEquity << "\n}";

    return os;
}

//------------------------------------------------------------------------------

sub_address_t sub_address_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    sub_address_t res = {};
    __bg_api_GET(subUid);
    __bg_api_GET(address);
    __bg_api_GET(chain);
    __bg_api_GET(coin);
    __bg_api_GET(tag);
    __bg_api_GET(url);
    
    return res;
}

std::ostream &operator<<(std::ostream &os, const sub_address_t &f) {
    os << "{\n"
    << "\t\"subUid\":" << f.subUid << ",\n"
    << "\t\"address\":" << f.address << ",\n"
    << "\t\"chain\":" << f.chain << ",\n"
    << "\t\"coin\":" << f.coin << ",\n"
    << "\t\"tag\":" << f.tag << ",\n"
    << "\t\"url\":" << f.url << "\n}";

    return os;
}

//------------------------------------------------------------------------------

sub_apikey_t sub_apikey_t::construct(const flatjson::fjson &json) {
    assert(json.is_valid());

    sub_apikey_t res = {};
    __bg_api_GET(subUid);
    __bg_api_GET(label);
    __bg_api_GET(apiKey);
    const auto arr = json.at("perms");
    assert(arr.is_array());
    for (auto idx = 0; idx < arr.size(); ++idx) {
        assert(arr[idx].is_string());
        std::string perm = arr[idx].to_string();
        res.perms.emplace(std::move(perm));
    }
    __bg_api_GET(ip);

    return res;
}

std::ostream &operator<<(std::ostream &os, const sub_apikey_t &f) {
    os << "{\n"
    << "\t\"subUid\":" << f.subUid << ",\n"
    << "\t\"label\":" << f.label << ",\n"
    << "\t\"apiKey\":" << f.apiKey << ",\n"
    << "\t\"perms\":[";
    for (auto &perm : f.perms) {
        os << perm << ",";
    }
    os << "],\n"
    << "\t\"ip\":" << f.ip << "\n}";

    return os;
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
