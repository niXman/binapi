
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

//#pragma warning(disable : 4715)

#include <bg_api/types.hpp>
#include <bg_api/fnv1a.hpp>
#include <bg_api/LUTs.hpp>

#include <type_traits>

//#include <iostream> // TODO: comment out

namespace bg_api {

//------------------------------------------------------------------------------

namespace rest {

//------------------------------------------------------------------------------

server_time_t server_time_t::construct(const simdjson::padded_string json) {
    simdjson::ondemand::parser parser;
    auto doc = parser.iterate(json);

    server_time_t res{};
    res.requestTime = doc["requestTime"];
    res.serverTime = doc["data"];

    return res;
}

std::ostream &operator<<(std::ostream &os, const server_time_t &f) {
    os << "{\n" << "\t\"serverTime\":" << f.serverTime << ",\n"
    << "\t\"requestTime\":" << f.requestTime << "\n}";

    return os;
}

//------------------------------------------------------------------------------

coin_t::chain_t coin_t::chain_t::construct(const simdjson::padded_string json) {
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

coin_t coin_t::construct(const simdjson::padded_string json) {
    simdjson::ondemand::parser parser;
    auto doc = parser.iterate(json);

    coin_t res{};
    res.coinId = doc["coinId"].get_uint64_in_string();
    res.coinName = doc["coinName"].get_string();
    res.transfer = doc["transfer"].get_bool();
    simdjson::ondemand::array arr(doc["chains"].get_array());
    for (auto x : arr) {
        coin_t::chain_t item{};
        item.chain = x["chain"].get_string();
        item.needTag = x["needTag"].get_bool();
        item.withdrawAble = x["withdrawAble"].get_bool();
        item.rechargeAble = x["rechargeAble"].get_bool();
        double_type withdraw_fee(static_cast<std::string>(x["withdrawFee"].get_string().take_value()));
        item.withdrawFee = withdraw_fee;
        item.depositConfirm = x["depositConfirm"].get_uint64();
        item.withdrawConfirm = x["withdrawConfirm"].get_uint64();
        double_type min_deposit_amount(static_cast<std::string>(x["minDepositAmount"].get_string().take_value()));
        item.minDepositAmount = min_deposit_amount;
        double_type min_withdraw_amount(static_cast<std::string>(x["minWithdrawAmount"].get_string().take_value()));
        item.minWithdrawAmount = min_withdraw_amount;
        item.browseUrl = x["browseUrl"].get_string();
        res.chains.emplace_back(std::move(item));
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

symbol_t symbol_t::construct(const simdjson::padded_string json) {
    simdjson::ondemand::parser parser;
    auto doc = parser.iterate(json);

    symbol_t res{};
    res.symbol = doc["symbol"].get_string();
    res.symbolName = doc["symbolName"].get_string();
    res.baseCoin = doc["baseCoin"].get_string();
    res.quoteCoin = doc["quoteCoin"].get_string();
    double_type min_trade_amount(static_cast<std::string>(doc["minTradeAmount"].get_string().take_value()));
    res.minTradeAmount = min_trade_amount;
    double_type max_trade_amount(static_cast<std::string>(doc["maxTradeAmount"].get_string().take_value()));
    res.maxTradeAmount = max_trade_amount;
    double_type taker_fee_rate(static_cast<std::string>(doc["takerFeeRate"].get_string().take_value()));
    res.takerFeeRate = taker_fee_rate;
    double_type maker_fee_rate(static_cast<std::string>(doc["makerFeeRate"].get_string().take_value()));
    res.makerFeeRate = maker_fee_rate;
    res.priceScale = doc["priceScale"].get_int64_in_string();
    res.quantityScale = doc["quantityScale"].get_int64_in_string();
    res.status = doc["status"].get_string();
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

spot_ticker_t spot_ticker_t::construct(const simdjson::padded_string json) {
    simdjson::ondemand::parser parser;
    auto doc = parser.iterate(json);

    spot_ticker_t res{};
    res.symbol = doc["symbol"].get_string();
    double_type high24h(static_cast<std::string>(doc["high24h"].get_string().take_value()));
    res.high24h = high24h;
    double_type low24h(static_cast<std::string>(doc["low24h"].get_string().take_value()));
    res.low24h = low24h;
    double_type close(static_cast<std::string>(doc["close"].get_string().take_value()));
    res.close = close;
    double_type quoteVol(static_cast<std::string>(doc["quoteVol"].get_string().take_value()));
    res.quoteVol = quoteVol;
    double_type baseVol(static_cast<std::string>(doc["baseVol"].get_string().take_value()));
    res.baseVol = baseVol;
    double_type usdtVol(static_cast<std::string>(doc["usdtVol"].get_string().take_value()));
    res.usdtVol = usdtVol;
    res.ts = doc["ts"].get_uint64_in_string();
    double_type buyOne(static_cast<std::string>(doc["buyOne"].get_string().take_value()));
    res.buyOne = buyOne;
    double_type sellOne(static_cast<std::string>(doc["sellOne"].get_string().take_value()));
    res.sellOne = sellOne;
    double_type bidSz(static_cast<std::string>(doc["bidSz"].get_string().take_value()));
    res.bidSz = bidSz;
    double_type askSz(static_cast<std::string>(doc["askSz"].get_string().take_value()));
    res.askSz = askSz;
    double_type openUtc0(static_cast<std::string>(doc["openUtc0"].get_string().take_value()));
    res.openUtc0 = openUtc0;

    return res;
}

std::ostream &operator<<(std::ostream &os, const spot_ticker_t &f) {
    os << "{\n"
    << "\t\"symbol\":" << f.symbol << ",\n"
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
    << "\t\"openUtc0\":" << f.openUtc0 << "\n}";

    return os;
}

//------------------------------------------------------------------------------

trade_t trade_t::construct(const simdjson::padded_string json) {
    simdjson::ondemand::parser parser;
    auto doc = parser.iterate(json);

    trade_t res{};
    res.symbol = doc["symbol"].get_string();
    res.tradeId = doc["tradeId"].get_string();
    res.side = side_from_string(doc["side"].get_string());
    double_type fillPrice(static_cast<std::string>(doc["fillPrice"].get_string().take_value()));
    res.fillPrice = fillPrice;
    double_type fillQuantity(static_cast<std::string>(doc["fillQuantity"].get_string().take_value()));
    res.fillQuantity = fillQuantity;
    res.fillTime = doc["fillTime"].get_uint64_in_string();

    return res;
}

std::ostream &operator<<(std::ostream &os, const trade_t &f) {
    os << "{\n"
    << "\t\"symbol\":" << f.symbol << ",\n"
    << "\t\"tradeId\":" << f.tradeId << ",\n"
    << "\t\"side\":" << side_to_string(f.side) << ",\n"
    << "\t\"fillPrice\":" << f.fillPrice << ",\n"
    << "\t\"fillQuantity\":" << f.fillQuantity << ",\n"
    << "\t\"fillTime\":" << f.fillTime << "\n}";

    return os;
}

//------------------------------------------------------------------------------

candle_t candle_t::construct_spot(const simdjson::padded_string json) {
    simdjson::ondemand::parser parser;
    auto doc = parser.iterate(json);

    candle_t res{};
    double_type open(static_cast<std::string>(doc["open"].get_string().take_value()));
    res.open = open;
    double_type high(static_cast<std::string>(doc["high"].get_string().take_value()));
    res.high = high;
    double_type low(static_cast<std::string>(doc["low"].get_string().take_value()));
    res.low = low;
    double_type close(static_cast<std::string>(doc["close"].get_string().take_value()));
    res.close = close;
    double_type baseVol(static_cast<std::string>(doc["baseVol"].get_string().take_value()));
    res.baseVol = baseVol;
    double_type quoteVol(static_cast<std::string>(doc["quoteVol"].get_string().take_value()));
    res.quoteVol = quoteVol;
    res.ts = doc["ts"].get_uint64_in_string();

    return res;
}

candle_t candle_t::construct_futures(const simdjson::padded_string json) {
    simdjson::ondemand::parser parser;
    auto doc = parser.iterate(json);

    candle_t res{};
    res.ts = doc[0].get_uint64_in_string();
    double_type open(static_cast<std::string>(doc.at(1).get_string().take_value()));
    res.open = open;
    double_type high(static_cast<std::string>(doc.at(2).get_string().take_value()));
    res.high = high;
    double_type low(static_cast<std::string>(doc.at(3).get_string().take_value()));
    res.low = low;
    double_type close(static_cast<std::string>(doc.at(4).get_string().take_value()));
    res.close = close;
    double_type baseVol(static_cast<std::string>(doc.at(5).get_string().take_value()));
    res.baseVol = baseVol;
    double_type quoteVol(static_cast<std::string>(doc.at(6).get_string().take_value()));
    res.quoteVol = quoteVol;

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

depth_t depth_t::construct(const simdjson::padded_string json) {
    simdjson::ondemand::parser parser;
    auto doc = parser.iterate(json);

    depth_t res = {};
    res.timestamp = doc["timestamp"].get_uint64_in_string();
    simdjson::ondemand::array asks(doc["asks"].get_array());
    for (auto ask : asks) {
        double_type price(static_cast<std::string>(ask.at(0).get_string().take_value()));
        double_type quantity(static_cast<std::string>(ask.at(1).get_string().take_value()));
        res.asks.emplace(std::move(price), std::move(quantity));
    }
    simdjson::ondemand::array bids(doc["bids"].get_array());
    for (auto bid : doc["bids"].get_array()) {
        double_type price(static_cast<std::string>(bid.at(0).get_string().take_value()));
        double_type quantity(static_cast<std::string>(bid.at(1).get_string().take_value()));
        res.bids.emplace(std::move(price), std::move(quantity));
    }

    return res;
}

std::ostream &operator<<(std::ostream &os, const depth_t &f) {
    os << "{\n" << "\t\"asks\":[\n";
    for (const auto &a : f.asks) {
        os << "\t\t[" << a.first << ", " << a.second << "],\n";
    }
    os << "\t],\n"
    << "\t\"bids\":[\n";
    for (const auto &b : f.bids) {
        os << "\t\t[" << b.first << ", " << b.second << "],\n";
    }
    os << "\t\"timestamp\":" << f.timestamp << "\t]\n}";

    return os;
}

//------------------------------------------------------------------------------

address_t address_t::construct(const simdjson::padded_string json) {
    simdjson::ondemand::parser parser;
    auto doc = parser.iterate(json);

    address_t res = {};
    res.address = doc["address"].get_string();
    res.coin = doc["coin"].get_string();
    res.chain = doc["chain"].get_string();
    res.tag = doc["tag"].get_string();
    res.url = doc["url"].get_string();

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

deposit_withdrawal_t deposit_withdrawal_t::construct(const simdjson::padded_string json) {
    simdjson::ondemand::parser parser;
    auto doc = parser.iterate(json);

    deposit_withdrawal_t res = {};
    res.id = doc["id"].get_string();
    res.txId = doc["txId"].get_string();
    res.coin = doc["coin"].get_string();
    res.type = doc["type"].get_string();
    double_type amount(static_cast<std::string>(doc["amount"].get_string().take_value()));
    res.amount = amount;
    res.status = doc["status"].get_string();
    res.toAddress = doc["toAddress"].get_string();
    res.chain = doc["chain"].get_string();
    res.cTime = doc["cTime"].get_uint64_in_string();
    res.uTime = doc["uTime"].get_uint64_in_string();

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
    << "\t\"chain\":" << f.chain << ",\n"
    << "\t\"cTime\":" << f.cTime << ",\n"
    << "\t\"uTime\":" << f.uTime << "\n}";

    return os;
}

//------------------------------------------------------------------------------

apikey_t apikey_t::construct(const simdjson::padded_string json) {
    simdjson::ondemand::parser parser;
    auto doc = parser.iterate(json);

    apikey_t res = {};
    res.userId = doc["userId"].get_string();
    res.inviterId = doc["inviterId"].get_string();
    res.ips = doc["ips"].get_string();
    simdjson::ondemand::array auths(doc["auths"].get_array());
    for (auto auth : doc["auths"].get_array()) {
        res.auths.emplace(static_cast<std::string>(auth.get_string().take_value()));
    }
    res.parentId = doc["parentId"].get_string();
    res.trader = doc["trader"].get_bool();

    return res;
}

std::ostream &operator<<(std::ostream &os, const apikey_t &f) {
    os << "{\n"
    << "\t\"userId\":" << f.userId << ",\n"
    << "\t\"inviterId\":" << f.inviterId << ",\n"
    << "\t\"ips\":" << f.ips << ",\n"
    << "\t\"auths\":[\n";
    for (const auto &a : f.auths) {
        os << "\t\t" << a << ",\n";
    }
    os << "\t],\n"
    << "\t\"parentId\":" << f.parentId << ",\n"
    << "\t\"trader\":" << f.trader << "\n}";

    return os;
}

//------------------------------------------------------------------------------

spot_account_t spot_account_t::construct(const simdjson::padded_string json) {
    simdjson::ondemand::parser parser;
    auto doc = parser.iterate(json);

    spot_account_t res = {};
    res.coinId = doc["coinId"].get_uint64_in_string();
    res.coinName = doc["coinName"].get_string();
    double_type available(static_cast<std::string>(doc["available"].get_string().take_value()));
    res.available = available;
    double_type lock(static_cast<std::string>(doc["lock"].get_string().take_value()));
    res.lock = lock;

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

bill_t bill_t::construct(const simdjson::padded_string json) {
    simdjson::ondemand::parser parser;
    auto doc = parser.iterate(json);

    bill_t res = {};
    res.cTime = doc["cTime"].get_uint64_in_string();
    res.coinId = doc["coinId"].get_uint64_in_string();
    res.coinName = doc["coinName"].get_string();
    res.groupType = group_type_from_string(doc["groupType"].get_string());
    res.bizType = biz_type_from_string(doc["bizType"].get_string());
    double_type quantity(static_cast<std::string>(doc["quantity"].get_string().take_value()));
    res.quantity = quantity;
    double_type balance(static_cast<std::string>(doc["balance"].get_string().take_value()));
    res.balance = balance;
    double_type fees(static_cast<std::string>(doc["fees"].get_string().take_value()));
    res.fees = fees;
    res.billId = doc["billId"].get_uint64_in_string();

    return res;
}

std::ostream &operator<<(std::ostream &os, const bill_t &f) {
    os << "{\n"
    << "\t\"cTime\":" << f.cTime << ",\n"
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

transfer_t transfer_t::construct(const simdjson::padded_string json) {
    simdjson::ondemand::parser parser;
    auto doc = parser.iterate(json);

    transfer_t res = {};
    res.coinName = doc["coinName"].get_string();
    res.status = doc["status"].get_string();
    res.toType = doc["toType"].get_string();
    res.toSymbol = doc["toSymbol"].get_string();
    res.fromType = doc["fromType"].get_string();
    res.fromSymbol = doc["fromSymbol"].get_string();
    double_type amount(static_cast<std::string>(doc["amount"].get_string().take_value()));
    res.amount = amount;
    res.tradeTime = doc["tradeTime"].get_uint64_in_string();

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

spot_order_t spot_order_t::construct(const simdjson::padded_string json) {
    simdjson::ondemand::parser parser;
    auto doc = parser.iterate(json);

    spot_order_t res = {};
    res.accountId = doc["accountId"].get_uint64_in_string();
    res.symbol = doc["symbol"].get_string();
    res.orderId = doc["orderId"].get_string();
    res.clientOrderId = doc["clientOrderId"].get_string();
    double_type price(static_cast<std::string>(doc["price"].get_string().take_value()));
    res.price = price;
    double_type quantity(static_cast<std::string>(doc["quantity"].get_string().take_value()));
    res.quantity = quantity;
    res.orderType = order_type_from_string(doc["orderType"].get_string());
    res.side = side_from_string(doc["side"].get_string());
    res.status = status_from_string(doc["status"].get_string());
    double_type fillPrice(static_cast<std::string>(doc["fillPrice"].get_string().take_value()));
    res.fillPrice = fillPrice;
    double_type fillQuantity(static_cast<std::string>(doc["fillQuantity"].get_string().take_value()));
    res.fillQuantity = fillQuantity;
    double_type fillTotalAmount(static_cast<std::string>(doc["fillTotalAmount"].get_string().take_value()));
    res.fillTotalAmount = fillTotalAmount;
    res.cTime = doc["cTime"].get_uint64_in_string();

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

transaction_t transaction_t::construct(const simdjson::padded_string json) {
    simdjson::ondemand::parser parser;
    auto doc = parser.iterate(json);

    transaction_t res = {};
    res.accountId = doc["accountId"].get_uint64_in_string();
    res.symbol = doc["symbol"].get_string();
    res.orderId = doc["orderId"].get_string();
    res.fillId = doc["fillId"].get_string();
    res.orderType = order_type_from_string(doc["orderType"].get_string());
    res.side = side_from_string(doc["side"].get_string());
    double_type fillPrice(static_cast<std::string>(doc["fillPrice"].get_string().take_value()));
    res.fillPrice = fillPrice;
    double_type fillQuantity(static_cast<std::string>(doc["fillQuantity"].get_string().take_value()));
    res.fillQuantity = fillQuantity;
    double_type fillTotalAmount(static_cast<std::string>(doc["fillTotalAmount"].get_string().take_value()));
    res.fillTotalAmount = fillTotalAmount;
    res.cTime = doc["cTime"].get_uint64_in_string();
    res.feeCcy = doc["feeCcy"].get_string();
    double_type fees(static_cast<std::string>(doc["fees"].get_string().take_value()));
    res.fees = fees;

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

spot_plan_order_t spot_plan_order_t::construct(const simdjson::padded_string json) {
    simdjson::ondemand::parser parser;
    auto doc = parser.iterate(json);

    spot_plan_order_t res = {};
    res.orderId = doc["orderId"].get_string();
    res.symbol = doc["symbol"].get_string();
    double_type size(static_cast<std::string>(doc["size"].get_string().take_value()));
    res.size = size;
    double_type executePrice(static_cast<std::string>(doc["executePrice"].get_string().take_value()));
    res.executePrice = executePrice;
    double_type triggerPrice(static_cast<std::string>(doc["triggerPrice"].get_string().take_value()));
    res.triggerPrice = triggerPrice;
    res.status = plan_status_from_string(doc["status"].get_string());
    res.orderType = order_type_from_string(doc["orderType"].get_string());
    res.side = side_from_string(doc["side"].get_string());
    res.cTime = doc["cTime"].get_uint64_in_string();

    return res;
}

std::ostream &operator<<(std::ostream &os, const spot_plan_order_t &f) {
    os << "{\n"
    << "\t\"orderId\":" << f.orderId << ",\n"
    << "\t\"symbol\":" << f.symbol << ",\n"
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

contract_t contract_t::construct(const simdjson::padded_string json) {
    simdjson::ondemand::parser parser;
    auto doc = parser.iterate(json);

    contract_t res = {};
    res.baseCoin = doc["baseCoin"].get_string();
    res.quoteCoin = doc["quoteCoin"].get_string();
    res.symbol = doc["symbol"].get_string();
    double_type buyLimitPriceRatio(static_cast<std::string>(doc["buyLimitPriceRatio"].get_string().take_value()));
    res.buyLimitPriceRatio = buyLimitPriceRatio;
    double_type sellLimitPriceRatio(static_cast<std::string>(doc["sellLimitPriceRatio"].get_string().take_value()));
    res.sellLimitPriceRatio = sellLimitPriceRatio;
    double_type feeRateUpRatio(static_cast<std::string>(doc["feeRateUpRatio"].get_string().take_value()));
    res.feeRateUpRatio = feeRateUpRatio;
    double_type openCostUpRatio(static_cast<std::string>(doc["openCostUpRatio"].get_string().take_value()));
    res.openCostUpRatio = openCostUpRatio;
    double_type sizeMultiplier(static_cast<std::string>(doc["sizeMultiplier"].get_string().take_value()));
    res.sizeMultiplier = sizeMultiplier;
    double_type makerFeeRate(static_cast<std::string>(doc["makerFeeRate"].get_string().take_value()));
    res.makerFeeRate = makerFeeRate;
    double_type takerFeeRate(static_cast<std::string>(doc["takerFeeRate"].get_string().take_value()));
    res.takerFeeRate = takerFeeRate;
    double_type minTradeNum(static_cast<std::string>(doc["minTradeNum"].get_string().take_value()));
    res.minTradeNum = minTradeNum;
    res.priceEndStep = doc["priceEndStep"].get_uint64_in_string();
    res.pricePlace = doc["pricePlace"].get_uint64_in_string();
    res.volumePlace = doc["volumePlace"].get_uint64_in_string();
    res.symbolType = doc["symbolType"].get_string();
    simdjson::ondemand::array arr = doc["supportMarginCoins"].get_array();
    for (auto coin : arr) {
        res.supportMarginCoins.push_back(static_cast<std::string>(coin.get_string().take_value()));
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

fill_t fill_t::construct(const simdjson::padded_string json) {
    simdjson::ondemand::parser parser;
    auto doc = parser.iterate(json);

    fill_t res = {};
    res.tradeId = doc["tradeId"].get_string();
    double_type price(static_cast<std::string>(doc["price"].get_string().take_value()));
    res.price = price;
    double_type size(static_cast<std::string>(doc["size"].get_string().take_value()));
    res.size = size;
    res.side = side_from_string(doc["side"].get_string());
    res.timestamp = doc["timestamp"].get_uint64_in_string();
    res.symbol = doc["symbol"].get_string();
    
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

symbol_leverage_t symbol_leverage_t::construct(const simdjson::padded_string json) {
    simdjson::ondemand::parser parser;
    auto doc = parser.iterate(json);

    symbol_leverage_t res = {};
    res.symbol = doc["symbol"].get_string();
    res.minLeverage = doc["minLeverage"].get_uint64_in_string();
    res.maxLeverage = doc["maxLeverage"].get_uint64_in_string();

    return res;
}

std::ostream &operator<<(std::ostream &os, const symbol_leverage_t &f) {
    os << "{\n"
    << "\t\"symbol\":" << f.symbol << ",\n"
    << "\t\"minLeverage\":" << f.minLeverage << ",\n"
    << "\t\"maxLeverage\":" << f.maxLeverage << "\n}";

    return os;
}

//------------------------------------------------------------------------------

futures_account_t futures_account_t::construct(const simdjson::padded_string json) {
    simdjson::ondemand::parser parser;
    auto doc = parser.iterate(json);

    futures_account_t res = {};
    res.marginCoin = doc["marginCoin"].get_string();
    double_type locked(doc["locked"].get_double().take_value());
    res.locked = locked;
    double_type available(doc["available"].get_double().take_value());
    res.available = available;
    double_type crossMaxAvailable(doc["crossMaxAvailable"].get_double().take_value());
    res.crossMaxAvailable = crossMaxAvailable;
    double_type fixedMaxAvailable(doc["fixedMaxAvailable"].get_double().take_value());
    res.fixedMaxAvailable = fixedMaxAvailable;
    double_type maxTransferOut(doc["maxTransferOut"].get_double().take_value());
    res.maxTransferOut = maxTransferOut;
    double_type equity(doc["equity"].get_double().take_value());
    res.equity = equity;
    double_type usdtEquity(doc["usdtEquity"].get_double().take_value());
    res.usdtEquity = usdtEquity;
    double_type btcEquity(doc["btcEquity"].get_double().take_value());
    res.btcEquity = btcEquity;
    double_type crossRiskRate(doc["crossRiskRate"].get_double().take_value());
    res.crossRiskRate = crossRiskRate;
    res.crossMarginLeverage = doc["crossMarginLeverage"].get_uint64();
    res.fixedLongLeverage = doc["fixedLongLeverage"].get_uint64();
    res.fixedShortLeverage = doc["fixedShortLeverage"].get_uint64();
    res.marginMode = margin_mode_from_string(doc["marginMode"].get_string());
    res.holdMode = hold_mode_from_string(doc["holdMode"].get_string());

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

futures_accounts_t futures_accounts_t::construct(const simdjson::padded_string json) {
    simdjson::ondemand::parser parser;
    auto doc = parser.iterate(json);

    futures_accounts_t res = {};
    res.marginCoin = doc["marginCoin"].get_string();
    double_type locked(doc["locked"].get_double().take_value());
    res.locked = locked;
    double_type available(doc["available"].get_double().take_value());
    res.available = available;
    double_type crossMaxAvailable(doc["crossMaxAvailable"].get_double().take_value());
    res.crossMaxAvailable = crossMaxAvailable;
    double_type fixedMaxAvailable(doc["fixedMaxAvailable"].get_double().take_value());
    res.fixedMaxAvailable = fixedMaxAvailable;
    double_type maxTransferOut(doc["maxTransferOut"].get_double().take_value());
    res.maxTransferOut = maxTransferOut;
    double_type equity(doc["equity"].get_double().take_value());
    res.equity = equity;
    double_type usdtEquity(doc["usdtEquity"].get_double().take_value());
    res.usdtEquity = usdtEquity;
    double_type btcEquity(doc["btcEquity"].get_double().take_value());
    res.btcEquity = btcEquity;  

    return res;
}

std::ostream &operator<<(std::ostream &os, const futures_accounts_t &f) {
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

position_t position_t::construct(const simdjson::padded_string json) {
    simdjson::ondemand::parser parser;
    auto doc = parser.iterate(json);

    position_t res = {};
    res.marginCoin = doc["marginCoin"].get_string();
    res.symbol = doc["symbol"].get_string();
    res.holdSide = hold_side_from_string(doc["holdSide"].get_string());
    double_type openDelegateCount(static_cast<std::string>(doc["openDelegateCount"].get_string().take_value()));
    res.openDelegateCount = openDelegateCount;
    double_type margin(static_cast<std::string>(doc["margin"].get_string().take_value()));
    res.margin = margin;
    double_type available(static_cast<std::string>(doc["available"].get_string().take_value()));
    res.available = available;
    double_type locked(static_cast<std::string>(doc["locked"].get_string().take_value()));
    res.locked = locked;
    double_type total(static_cast<std::string>(doc["total"].get_string().take_value()));
    res.total = total;
    res.leverage = doc["leverage"].get_uint64();
    double_type achievedProfits(static_cast<std::string>(doc["achievedProfits"].get_string().take_value()));
    res.achievedProfits = achievedProfits;
    double_type averageOpenPrice(static_cast<std::string>(doc["averageOpenPrice"].get_string().take_value()));
    res.averageOpenPrice = averageOpenPrice;
    res.marginMode = margin_mode_from_string(doc["marginMode"].get_string());
    res.holdMode = hold_mode_from_string(doc["holdMode"].get_string());
    double_type unrealizedPL(static_cast<std::string>(doc["unrealizedPL"].get_string().take_value()));
    res.unrealizedPL = unrealizedPL;
    double_type keepMarginRate(static_cast<std::string>(doc["keepMarginRate"].get_string().take_value()));
    res.keepMarginRate = keepMarginRate;
    double_type marketPrice(static_cast<std::string>(doc["marketPrice"].get_string().take_value()));
    res.marketPrice = marketPrice;
    res.cTime = doc["ctime"].get_uint64();

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

futures_bill_t futures_bill_t::construct(const simdjson::padded_string json) {
    simdjson::ondemand::parser parser;
    auto doc = parser.iterate(json);

    futures_bill_t res = {};
    res.id = doc["id"].get_string();
    res.symbol = doc["symbol"].get_string();
    res.marginCoin = doc["marginCoin"].get_string();
    double_type amount(static_cast<std::string>(doc["amount"].get_string().take_value()));
    res.amount = amount;
    double_type fee(static_cast<std::string>(doc["fee"].get_string().take_value()));
    res.fee = fee;
    double_type feeByCoupon(static_cast<std::string>(doc["feeByCoupon"].get_string().take_value()));
    res.feeByCoupon = feeByCoupon;
    res.feeCoin = doc["feeCoin"].get_string();
    res.business = business_from_string(doc["business"].get_string());
    res.cTime = doc["ctime"].get_uint64();
    
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

coin_leverage_t coin_leverage_t::construct(const simdjson::padded_string json) {
    simdjson::ondemand::parser parser;
    auto doc = parser.iterate(json);

    coin_leverage_t res = {};
    res.symbol = doc["symbol"].get_string();
    res.marginCoin = doc["marginCoin"].get_string();
    res.longLeverage = doc["longLeverage"].get_uint64();
    res.shortLeverage = doc["shortLeverage"].get_uint64();
    res.marginMode = margin_mode_from_string(doc["marginMode"].get_string());

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

futures_open_order_t futures_open_order_t::construct(const simdjson::padded_string json) {
    simdjson::ondemand::parser parser;
    auto doc = parser.iterate(json);

    futures_open_order_t res = {};
    res.symbol = doc["symbol"].get_string();
    double_type size(doc["size"].get_double().take_value());
    res.size = size;
    res.orderId = doc["orderId"].get_string();
    res.clientOid = doc["clientOid"].get_string();
    double_type filledQty(doc["filledQty"].get_double().take_value());
    res.filledQty = filledQty;
    double_type fee(doc["fee"].get_double().take_value());
    res.fee = fee;
    double_type price(doc["price"].get_double().take_value());
    res.price = price;
    res.state = status_from_string(doc["state"].get_string());
    res.side = trade_side_from_string(doc["side"].get_string());
    res.timeInForce = force_from_string(doc["timeInForce"].get_string());
    double_type totalProfits(doc["totalProfits"].get_double().take_value());
    res.totalProfits = totalProfits;
    res.posSide = hold_side_from_string(doc["posSide"].get_string());
    res.marginCoin = doc["marginCoin"].get_string();
    res.orderType = order_type_from_string(doc["orderType"].get_string());
    res.cTime = doc["ctime"].get_uint64();

    return res;
}

std::ostream &operator<<(std::ostream &os, const futures_open_order_t &f) {
    os << "{\n"
    << "\t\"symbol\":" << f.symbol << ",\n"
    << "\t\"size\":" << f.size << ",\n"
    << "\t\"orderId\":" << f.orderId << ",\n"
    << "\t\"clientOid\":" << f.clientOid << ",\n"
    << "\t\"filledQty\":" << f.filledQty << ",\n"
    << "\t\"fee\":" << f.fee << ",\n"
    << "\t\"price\":" << f.price << ",\n"
    << "\t\"state\":" << status_to_string(f.state) << ",\n"
    << "\t\"side\":" << trade_side_to_string(f.side) << ",\n"
    << "\t\"timeInForce\":" << force_to_string(f.timeInForce) << ",\n"
    << "\t\"totalProfits\":" << f.totalProfits << ",\n"
    << "\t\"posSide\":" << hold_side_to_string(f.posSide) << ",\n"
    << "\t\"marginCoin\":" << f.marginCoin << ",\n"
    << "\t\"orderType\":" << order_type_to_string(f.orderType) << ",\n"
    << "\t\"cTime\":" << f.cTime << "\n}";

    return os;
}

//------------------------------------------------------------------------------

futures_history_order_t futures_history_order_t::construct(const simdjson::padded_string json) {
    simdjson::ondemand::parser parser;
    auto doc = parser.iterate(json);

    futures_history_order_t res = {};
    res.symbol = doc["symbol"].get_string();
    double_type size(doc["size"].get_double().take_value());
    res.size = size;
    res.orderId = doc["orderId"].get_string();
    res.clientOid = doc["clientOid"].get_string();
    double_type filledQty(doc["filledQty"].get_double().take_value());
    res.filledQty = filledQty;
    double_type fee(doc["fee"].get_double().take_value());
    res.fee = fee;
    double_type price(doc["price"].get_double().take_value());
    res.price = price;
    double_type priceAvg(doc["priceAvg"].get_double().take_value());
    res.priceAvg = priceAvg;
    res.state = status_from_string(doc["state"].get_string());
    res.side = trade_side_from_string(doc["side"].get_string());
    res.timeInForce = force_from_string(doc["timeInForce"].get_string());
    double_type totalProfits(doc["totalProfits"].get_double().take_value());
    res.totalProfits = totalProfits;
    res.posSide = hold_side_from_string(doc["posSide"].get_string());
    res.marginCoin = doc["marginCoin"].get_string();
    double_type filledAmount(doc["filledAmount"].get_double().take_value());
    res.filledAmount = filledAmount;
    res.orderType = order_type_from_string(doc["orderType"].get_string());
    res.leverage = doc["leverage"].get_uint64_in_string();
    res.marginMode = margin_mode_from_string(doc["marginMode"].get_string());
    res.cTime = doc["cTime"].get_uint64();
    res.uTime = doc["uTime"].get_uint64();

    return res;
}

std::ostream &operator<<(std::ostream &os, const futures_history_order_t &f) {
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
    << "\t\"side\":" << trade_side_to_string(f.side) << ",\n"
    << "\t\"timeInForce\":" << force_to_string(f.timeInForce) << ",\n"
    << "\t\"totalProfits\":" << f.totalProfits << ",\n"
    << "\t\"posSide\":" << hold_side_to_string(f.posSide) << ",\n"
    << "\t\"marginCoin\":" << f.marginCoin << ",\n"
    << "\t\"filledAmount\":" << f.filledAmount << ",\n"
    << "\t\"orderType\":" << order_type_to_string(f.orderType) << ",\n"
    << "\t\"leverage\":" << f.leverage << ",\n"
    << "\t\"marginMode\":" << margin_mode_to_string(f.marginMode) << ",\n"
    << "\t\"cTime\":" << f.cTime << ",\n"
    << "\t\"uTime\":" << f.uTime << "\n}";

    return os;
}

//------------------------------------------------------------------------------

futures_product_type_history_t futures_product_type_history_t::construct(const simdjson::padded_string json) {
    simdjson::ondemand::parser parser;
    auto doc = parser.iterate(json);

    futures_product_type_history_t res = {};
    res.symbol = doc["symbol"].get_string();
    double_type size(doc["size"].get_double().take_value());
    res.size = size;
    res.orderId = doc["orderId"].get_string();
    res.clientOid = doc["clientOid"].get_string();
    double_type filledQty(doc["filledQty"].get_double().take_value());
    res.filledQty = filledQty;
    double_type fee(doc["fee"].get_double().take_value());
    res.fee = fee;
    double_type price(doc["price"].get_double().take_value());
    res.price = price;
    res.state = status_from_string(doc["state"].get_string());
    res.side = trade_side_from_string(doc["side"].get_string());
    res.timeInForce = force_from_string(doc["timeInForce"].get_string());
    double_type totalProfits(doc["totalProfits"].get_double().take_value());
    res.totalProfits = totalProfits;
    res.posSide = hold_side_from_string(doc["posSide"].get_string());
    res.marginCoin = doc["marginCoin"].get_string();
    res.leverage = doc["leverage"].get_uint64_in_string();
    res.marginMode = margin_mode_from_string(doc["marginMode"].get_string());
    res.orderType = order_type_from_string(doc["orderType"].get_string());
    res.cTime = doc["cTime"].get_uint64();

    return res;
}

std::ostream &operator<<(std::ostream &os, const futures_product_type_history_t &f) {
    os << "{\n"
    << "\t\"symbol\":" << f.symbol << ",\n"
    << "\t\"size\":" << f.size << ",\n"
    << "\t\"orderId\":" << f.orderId << ",\n"
    << "\t\"clientOid\":" << f.clientOid << ",\n"
    << "\t\"filledQty\":" << f.filledQty << ",\n"
    << "\t\"fee\":" << f.fee << ",\n"
    << "\t\"price\":" << f.price << ",\n"
    << "\t\"state\":" << status_to_string(f.state) << ",\n"
    << "\t\"side\":" << trade_side_to_string(f.side) << ",\n"
    << "\t\"timeInForce\":" << force_to_string(f.timeInForce) << ",\n"
    << "\t\"totalProfits\":" << f.totalProfits << ",\n"
    << "\t\"posSide\":" << hold_side_to_string(f.posSide) << ",\n"
    << "\t\"marginCoin\":" << f.marginCoin << ",\n"
    << "\t\"leverage\":" << f.leverage << ",\n"
    << "\t\"marginMode\":" << margin_mode_to_string(f.marginMode) << ",\n"
    << "\t\"orderType\":" << order_type_to_string(f.orderType) << ",\n"
    << "\t\"cTime\":" << f.cTime << "\n}";

    return os;
}

//------------------------------------------------------------------------------

futures_order_details_t futures_order_details_t::construct(const simdjson::padded_string json) {
    simdjson::ondemand::parser parser;
    auto doc = parser.iterate(json);

    futures_order_details_t res = {};
    res.symbol = doc["symbol"].get_string();
    double_type size(doc["size"].get_double().take_value());
    res.size = size;
    res.orderId = doc["orderId"].get_string();
    res.clientOid = doc["clientOid"].get_string();
    double_type filledQty(doc["filledQty"].get_double().take_value());
    res.filledQty = filledQty;
    double_type fee(doc["fee"].get_double().take_value());
    res.fee = fee;
    double_type price(doc["price"].get_double().take_value());
    res.price = price;
    res.state = status_from_string(doc["state"].get_string());
    res.side = trade_side_from_string(doc["side"].get_string());
    res.timeInForce = force_from_string(doc["timeInForce"].get_string());
    double_type totalProfits(doc["totalProfits"].get_double().take_value());
    res.totalProfits = totalProfits;
    res.posSide = hold_side_from_string(doc["posSide"].get_string());
    res.marginCoin = doc["marginCoin"].get_string();
    double_type presetTakeProfitPrice(doc["presetTakeProfitPrice"].get_double().take_value());
    res.presetTakeProfitPrice = presetTakeProfitPrice;
    double_type presetStopLossPrice(doc["presetStopLossPrice"].get_double().take_value());
    res.presetStopLossPrice = presetStopLossPrice;
    double_type filledAmount(doc["filledAmount"].get_double().take_value());
    res.filledAmount = filledAmount;
    res.orderType = order_type_from_string(doc["orderType"].get_string());
    res.cTime = doc["cTime"].get_uint64();
    res.uTime = doc["uTime"].get_uint64();

    return res;
}

std::ostream &operator<<(std::ostream &os, const futures_order_details_t &f) {
    os << "{\n"
    << "\t\"symbol\":" << f.symbol << ",\n"
    << "\t\"size\":" << f.size << ",\n"
    << "\t\"orderId\":" << f.orderId << ",\n"
    << "\t\"clientOid\":" << f.clientOid << ",\n"
    << "\t\"filledQty\":" << f.filledQty << ",\n"
    << "\t\"fee\":" << f.fee << ",\n"
    << "\t\"price\":" << f.price << ",\n"
    << "\t\"state\":" << status_to_string(f.state) << ",\n"
    << "\t\"side\":" << trade_side_to_string(f.side) << ",\n"
    << "\t\"timeInForce\":" << force_to_string(f.timeInForce) << ",\n"
    << "\t\"totalProfits\":" << f.totalProfits << ",\n"
    << "\t\"posSide\":" << hold_side_to_string(f.posSide) << ",\n"
    << "\t\"marginCoin\":" << f.marginCoin << ",\n"
    << "\t\"presetTakeProfitPrice\":" << f.presetTakeProfitPrice << ",\n"
    << "\t\"presetStopLossPrice\":" << f.presetStopLossPrice << ",\n"
    << "\t\"filledAmount\":" << f.filledAmount << ",\n"
    << "\t\"orderType\":" << order_type_to_string(f.orderType) << ",\n"
    << "\t\"cTime\":" << f.cTime << ",\n"
    << "\t\"uTime\":" << f.uTime << "\n}";

    return os;
}

//------------------------------------------------------------------------------

futures_plan_order_t futures_plan_order_t::construct(const simdjson::padded_string json) {
    simdjson::ondemand::parser parser;
    auto doc = parser.iterate(json);

    futures_plan_order_t res = {};
    res.orderId = doc["orderId"].get_string();
    res.symbol = doc["symbol"].get_string();
    res.marginCoin = doc["marginCoin"].get_string();
    double_type size(static_cast<std::string>(doc["size"].get_string().take_value()));
    res.size = size;
    double_type executePrice(static_cast<std::string>(doc["executePrice"].get_string().take_value()));
    res.executePrice = executePrice;
    double_type triggerPrice(static_cast<std::string>(doc["triggerPrice"].get_string().take_value()));
    res.triggerPrice = triggerPrice;
    res.status = plan_status_from_string(doc["status"].get_string());
    res.orderType = order_type_from_string(doc["orderType"].get_string());
    res.planType = plan_type_from_string(doc["planType"].get_string());
    res.side = trade_side_from_string(doc["side"].get_string());
    res.triggerType = trigger_type_from_string(doc["triggerType"].get_string());
    double_type presetTakeProfitPrice(static_cast<std::string>(doc["presetTakeProfitPrice"].get_string().take_value()));
    res.presetTakeProfitPrice = presetTakeProfitPrice;
    double_type presetStopLossPrice(static_cast<std::string>(doc["presetStopLossPrice"].get_string().take_value()));
    res.presetStopLossPrice = presetStopLossPrice;
    res.cTime = doc["ctime"].get_uint64();

    return res;
}

std::ostream &operator<<(std::ostream &os, const futures_plan_order_t &f) {
    os << "{\n"
    << "\t\"orderId\":" << f.orderId << ",\n"
    << "\t\"symbol\":" << f.symbol << ",\n"
    << "\t\"marginCoin\":" << f.marginCoin << ",\n"
    << "\t\"size\":" << f.size << ",\n"
    << "\t\"executePrice\":" << f.executePrice << ",\n"
    << "\t\"triggerPrice\":" << f.triggerPrice << ",\n"
    << "\t\"status\":" << plan_status_to_string(f.status) << ",\n"
    << "\t\"orderType\":" << order_type_to_string(f.orderType) << ",\n"
    << "\t\"side\":" << trade_side_to_string(f.side) << ",\n"
    << "\t\"triggerType\":" << trigger_type_to_string(f.triggerType) << ",\n"
    << "\t\"presetTakeProfitPrice\":" << f.presetTakeProfitPrice << ",\n"
    << "\t\"presetStopLossPrice\":" << f.presetStopLossPrice << "\n}";

    return os;
}

//------------------------------------------------------------------------------

futures_history_plan_order_t futures_history_plan_order_t::construct(const simdjson::padded_string json) {
    simdjson::ondemand::parser parser;
    auto doc = parser.iterate(json);

    futures_history_plan_order_t res = {};
    res.orderId = doc["orderId"].get_string();
    res.executeOrderId = doc["executeOrderId"].get_string();
    res.symbol = doc["symbol"].get_string();
    res.marginCoin = doc["marginCoin"].get_string();
    double_type size(static_cast<std::string>(doc["size"].get_string().take_value()));
    res.size = size;
    double_type executePrice(static_cast<std::string>(doc["executePrice"].get_string().take_value()));
    res.executePrice = executePrice;
    double_type triggerPrice(static_cast<std::string>(doc["triggerPrice"].get_string().take_value()));
    res.triggerPrice = triggerPrice;
    res.status = plan_status_from_string(doc["status"].get_string());
    res.orderType = order_type_from_string(doc["orderType"].get_string());
    res.planType = plan_type_from_string(doc["planType"].get_string());
    res.side = trade_side_from_string(doc["side"].get_string());
    res.triggerType = trigger_type_from_string(doc["triggerType"].get_string());
    double_type presetTakeProfitPrice(static_cast<std::string>(doc["presetTakeProfitPrice"].get_string().take_value()));
    res.presetTakeProfitPrice = presetTakeProfitPrice;
    double_type presetStopLossPrice(static_cast<std::string>(doc["presetStopLossPrice"].get_string().take_value()));
    res.presetStopLossPrice = presetStopLossPrice;
    res.cTime = doc["ctime"].get_uint64();

    return res;
}

std::ostream &operator<<(std::ostream &os, const futures_history_plan_order_t &f) {
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
    << "\t\"side\":" << trade_side_to_string(f.side) << ",\n"
    << "\t\"triggerType\":" << trigger_type_to_string(f.triggerType) << ",\n"
    << "\t\"presetTakeProfitPrice\":" << f.presetTakeProfitPrice << ",\n"
    << "\t\"presetStopLossPrice\":" << f.presetStopLossPrice << ",\n"
    << "\t\"ctime\":" << f.cTime << "\n}";

    return os;
}

//------------------------------------------------------------------------------

futures_fill_t futures_fill_t::construct(const simdjson::padded_string json) {
    simdjson::ondemand::parser parser;
    auto doc = parser.iterate(json);

    futures_fill_t res = {};
    res.tradeId = doc["tradeId"].get_string();
    res.symbol = doc["symbol"].get_string();
    res.orderId = doc["orderId"].get_string();
    double_type price(static_cast<std::string>(doc["price"].get_string().take_value()));
    res.price = price;
    double_type sizeQty(static_cast<std::string>(doc["sizeQty"].get_string().take_value()));
    res.sizeQty = sizeQty;
    double_type fee(static_cast<std::string>(doc["fee"].get_string().take_value()));
    res.fee = fee;
    res.side = trade_side_from_string(doc["side"].get_string());
    res.cTime = doc["ctime"].get_uint64();

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

} // ns rest

namespace ws {

//------------------------------------------------------------------------------

spot_ticker_t spot_ticker_t::construct(const simdjson::padded_string json) {
    simdjson::ondemand::parser parser;
    auto doc = parser.iterate(json);

    spot_ticker_t res = {};
    res.instId = doc["instId"].get_string();
    double_type last(static_cast<std::string>(doc["last"].get_string().take_value()));
    res.last = last;
    double_type bestAsk(static_cast<std::string>(doc["bestAsk"].get_string().take_value()));
    res.bestAsk = bestAsk;
    double_type bestBid(static_cast<std::string>(doc["bestBid"].get_string().take_value()));
    res.bestBid = bestBid;
    double_type open24h(static_cast<std::string>(doc["open24h"].get_string().take_value()));
    res.open24h = open24h;
    double_type high24h(static_cast<std::string>(doc["high24h"].get_string().take_value()));
    res.high24h = high24h;
    double_type low24h(static_cast<std::string>(doc["low24h"].get_string().take_value()));
    res.low24h = low24h;
    double_type baseVolume(static_cast<std::string>(doc["baseVolume"].get_string().take_value()));
    res.baseVolume = baseVolume;
    double_type quoteVolume(static_cast<std::string>(doc["quoteVolume"].get_string().take_value()));
    res.quoteVolume = quoteVolume;
    res.ts = doc["ts"].get_uint64_in_string();
    res.labelId = doc["labelId"].get_uint64();
    double_type openUtc(static_cast<std::string>(doc["openUtc"].get_string().take_value()));
    res.openUtc = openUtc;
    double_type bidSz(static_cast<std::string>(doc["bidSz"].get_string().take_value()));
    res.bidSz = bidSz;
    double_type askSz(static_cast<std::string>(doc["askSz"].get_string().take_value()));
    res.askSz = askSz;

    return res;
}

std::ostream &operator<<(std::ostream &os, const spot_ticker_t &f) {
    os << "{\n"
    << "\t\"instId\":" << f.instId << ",\n"
    << "\t\"last\":" << f.last << ",\n"
    << "\t\"bestAsk\":" << f.bestAsk << ",\n"
    << "\t\"bestBid\":" << f.bestBid << ",\n"
    << "\t\"open24h\":" << f.open24h << ",\n"
    << "\t\"high24h\":" << f.high24h << ",\n"
    << "\t\"low24h\":" << f.low24h << ",\n"
    << "\t\"bestBid\":" << f.bestBid << ",\n"
    << "\t\"bestAsk\":" << f.bestAsk << ",\n"
    << "\t\"baseVolume\":" << f.baseVolume << ",\n"
    << "\t\"quoteVolume\":" << f.quoteVolume << ",\n"
    << "\t\"ts\":" << f.ts << ",\n"
    << "\t\"labelId\":" << f.labelId << ",\n"
    << "\t\"openUtc\":" << f.openUtc << ",\n"
    << "\t\"chgUtc\":" << f.chgUtc << ",\n"
    << "\t\"bidSz\":" << f.bidSz << ",\n"
    << "\t\"askSz\":" << f.askSz << "\n}";
    
    return os;
}

//------------------------------------------------------------------------------

candle_t candle_t::construct(const simdjson::padded_string json) {
    simdjson::ondemand::parser parser;
    auto doc = parser.iterate(json);

    candle_t res = {};
    res.ts = doc.at(0).get_uint64_in_string();
    double_type open(static_cast<std::string>(doc.at(1).get_string().take_value()));
    res.open = open;
    double_type high(static_cast<std::string>(doc.at(2).get_string().take_value()));
    res.high = high;
    double_type low(static_cast<std::string>(doc.at(3).get_string().take_value()));
    res.low = low;
    double_type close(static_cast<std::string>(doc.at(4).get_string().take_value()));
    res.close = close;
    double_type volume(static_cast<std::string>(doc.at(5).get_string().take_value()));
    res.volume = volume;

    return res;
}

std::ostream &operator<<(std::ostream &os, const candle_t &f) {
    os << "{\n"
    << "\t\"ts\":" << f.ts << ",\n"
    << "\t\"open\":" << f.open << ",\n"
    << "\t\"high\":" << f.high << ",\n"
    << "\t\"low\":" << f.low << ",\n"
    << "\t\"close\":" << f.close << ",\n"
    << "\t\"volume\":" << f.volume << "\n}";

    return os;
}

//------------------------------------------------------------------------------

depth_t depth_t::construct(const simdjson::padded_string json) {
    simdjson::ondemand::parser parser;
    auto doc = parser.iterate(json);

    depth_t res = {};
    simdjson::ondemand::array asks = doc["asks"].get_array();
    for (auto ask : asks) {
        simdjson::ondemand::array a = ask.get_array();
        double_type price(static_cast<std::string>(a.at(0).get_string().take_value()));
        double_type size(static_cast<std::string>(a.at(1).get_string().take_value()));
        res.asks.emplace(std::make_pair(price, size));
    }
    simdjson::ondemand::array bids = doc["bids"].get_array();
    for (auto bid : bids) {
        simdjson::ondemand::array b = bid.get_array();
        double_type price(static_cast<std::string>(b.at(0).get_string().take_value()));
        double_type size(static_cast<std::string>(b.at(1).get_string().take_value()));
        res.bids.emplace(std::make_pair(price, size));
    }
    res.ts = doc["ts"].get_uint64_in_string();
    res.checksum = doc["checksum"].get_uint64();

    return res;
}

std::ostream &operator<<(std::ostream &os, const depth_t &f) {
    os << "{\n"
    << "\t\"ts\":" << f.ts << ",\n"
    << "\t\"checksum\":" << f.checksum << ",\n"
    << "\t\"asks\":[\n";
    for (auto &a : f.asks) {
        os << "\t\t[" << a.first << "," << a.second << "],\n";
    }
    os << "\t],\n"
    << "\t\"bids\":[\n";
    for (auto &b : f.bids) {
        os << "\t\t[" << b.first << "," << b.second << "],\n";
    }
    os << "\t]\n}";

    return os;
}

//------------------------------------------------------------------------------

trade_t trade_t::construct(const simdjson::padded_string json) {
    simdjson::ondemand::parser parser;
    auto doc = parser.iterate(json);

    trade_t res = {};
    res.ts = doc.at(0).get_uint64_in_string();
    double_type price(static_cast<std::string>(doc.at(1).get_string().take_value()));
    res.price = price;
    double_type size(static_cast<std::string>(doc.at(2).get_string().take_value()));
    res.size = size;
    res.side = side_from_string(doc.at(3).get_string());

    return res;
}

std::ostream &operator<<(std::ostream &os, const trade_t &f) {
    os << "{\n"
    << "\t\"ts\":" << f.ts << ",\n"
    << "\t\"price\":" << f.price << ",\n"
    << "\t\"size\":" << f.size << ",\n"
    << "\t\"side\":" << side_to_string(f.side) << "\n}";

    return os;
}

//------------------------------------------------------------------------------

futures_ticker_t futures_ticker_t::construct(const simdjson::padded_string json) {
    simdjson::ondemand::parser parser;
    auto doc = parser.iterate(json);

    futures_ticker_t res = {};
    res.instId = doc["instId"].get_string();
    double_type last(static_cast<std::string>(doc["last"].get_string().take_value()));
    res.last = last;
    double_type high24h(static_cast<std::string>(doc["high24h"].get_string().take_value()));
    res.high24h = high24h;
    double_type low24h(static_cast<std::string>(doc["low24h"].get_string().take_value()));
    res.low24h = low24h;
    double_type bestBid(static_cast<std::string>(doc["bestBid"].get_string().take_value()));
    res.bestBid = bestBid;
    double_type bestAsk(static_cast<std::string>(doc["bestAsk"].get_string().take_value()));
    res.bestAsk = bestAsk;
    double_type priceChangePct(static_cast<std::string>(doc["priceChangePercent"].get_string().take_value()));
    res.priceChangePct = priceChangePct;
    double_type capitalRate(static_cast<std::string>(doc["capitalRate"].get_string().take_value()));
    res.capitalRate = capitalRate;
    res.nextSettleTime = doc["nextSettleTime"].get_uint64();
    res.systemTime = doc["systemTime"].get_uint64();
    double_type markPrice(static_cast<std::string>(doc["markPrice"].get_string().take_value()));
    res.markPrice = markPrice;
    double_type indexPrice(static_cast<std::string>(doc["indexPrice"].get_string().take_value()));
    res.indexPrice = indexPrice;
    double_type holding(static_cast<std::string>(doc["holding"].get_string().take_value()));
    res.holding = holding;
    double_type baseVolume(static_cast<std::string>(doc["baseVolume"].get_string().take_value()));
    res.baseVolume = baseVolume;
    double_type quoteVolume(static_cast<std::string>(doc["quoteVolume"].get_string().take_value()));
    res.quoteVolume = quoteVolume;
    double_type openUtc(static_cast<std::string>(doc["openUtc"].get_string().take_value()));
    res.openUtc = openUtc;
    double_type chgUtc(static_cast<std::string>(doc["chgUTC"].get_string().take_value()));
    res.chgUtc = chgUtc;
    double_type bidSz(static_cast<std::string>(doc["bidSz"].get_string().take_value()));
    res.bidSz = bidSz;
    double_type askSz(static_cast<std::string>(doc["askSz"].get_string().take_value()));
    res.askSz = askSz;

    return res;
}

std::ostream &operator<<(std::ostream &os, const futures_ticker_t &f) {
    os << "{\n"
    << "\t\"instId\":" << f.instId << ",\n"
    << "\t\"last\":" << f.last << ",\n"
    << "\t\"high24h\":" << f.high24h << ",\n"
    << "\t\"low24h\":" << f.low24h << ",\n"
    << "\t\"bestBid\":" << f.bestBid << ",\n"
    << "\t\"bestAsk\":" << f.bestAsk << ",\n"
    << "\t\"priceChangePct\":" << f.priceChangePct << ",\n"
    << "\t\"capitalRate\":" << f.capitalRate << ",\n"
    << "\t\"nextSettleTime\":" << f.nextSettleTime << ",\n"
    << "\t\"systemTime\":" << f.systemTime << ",\n"
    << "\t\"markPrice\":" << f.markPrice << ",\n"
    << "\t\"indexPrice\":" << f.indexPrice << ",\n"
    << "\t\"holding\":" << f.holding << ",\n"
    << "\t\"baseVolume\":" << f.baseVolume << ",\n"
    << "\t\"quoteVolume\":" << f.quoteVolume << ",\n"
    << "\t\"openUtc\":" << f.openUtc << ",\n"
    << "\t\"chgUtc\":" << f.chgUtc << ",\n"
    << "\t\"bidSz\":" << f.bidSz << ",\n"
    << "\t\"askSz\":" << f.askSz << "\n}";

    return os;
}

//------------------------------------------------------------------------------

} // ns ws

} // ns bg_api