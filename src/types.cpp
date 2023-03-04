
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

#include <type_traits>

#include <iostream> // TODO: comment out

namespace bg_api {

//------------------------------------------------------------------------------

namespace rest {

//------------------------------------------------------------------------------

server_time_t server_time_t::construct(simdjson::ondemand::document &doc) {
    server_time_t res{};
    res.serverTime = doc["data"].get_uint64_in_string();

    return res;
}

std::ostream &operator<<(std::ostream &os, const server_time_t &f) {
    os << "{" << "\"serverTime\":" << f.serverTime << "}";

    return os;
}

//------------------------------------------------------------------------------

currencies_t::currency_t::chain_t currencies_t::currency_t::chain_t::construct(simdjson::ondemand::object &obj) {
    chain_t res = {};
    for (auto field : obj) {
        if (field.value().is_null()) continue;
        std::string_view key = field.unescaped_key();
        if (key == "chain") {
            res.chain = field.value().get_string();
        } else if (key == "needTag") {
            std::string_view value = field.value().get_string();
            const char* val = std::string(value).c_str();
            res.needTag = val == "true";
        } else if (key == "withdrawable") {
            std::string_view value = field.value().get_string();
            const char* val = std::string(value).c_str();
            res.withdrawable = val == "true";
        } else if (key == "rechargeable") {
            std::string_view value = field.value().get_string();
            const char* val = std::string(value).c_str();
            res.rechargeable = val == "true";
        } else if (key == "withdrawFee") {
            double_type withdrawFee(static_cast<std::string>(field.value().get_string().value()));
            res.withdrawFee = withdrawFee;
        } else if (key == "extraWithDrawFee") {
            double_type extraWithDrawFee(static_cast<std::string>(field.value().get_string().value()));
            res.extraWithDrawFee = extraWithDrawFee;
        } else if (key == "depositConfirm") {
            res.depositConfirm = field.value().get_uint64_in_string();
        } else if (key == "withdrawConfirm") {
            res.withdrawConfirm = field.value().get_uint64_in_string();
        } else if (key == "minWithdrawAmount") {
            double_type minDepositAmount(static_cast<std::string>(field.value().get_string().value()));
            res.minDepositAmount = minDepositAmount;
        } else if (key == "minWithdrawAmount") {
            double_type minWithdrawAmount(static_cast<std::string>(field.value().get_string().value()));
            res.minWithdrawAmount = minWithdrawAmount;
        } else if (key == "browserUrl") {
            res.browserUrl = field.value().get_string();
        }
    }

    return res;
}

std::ostream &operator<<(std::ostream &os, const currencies_t::currency_t::chain_t &f) {
    os << "{"
        << "\"chain\":" << f.chain << ","
        << "\"needTag\":" << std::boolalpha << f.needTag << ","
        << "\"withdrawable\":" << std::boolalpha << f.withdrawable << ","
        << "\"rechargeable\":" << std::boolalpha << f.rechargeable << ","
        << "\"withdrawFee\":" << f.withdrawFee << ","
        << "\"extraWithDrawFee\":" << f.extraWithDrawFee << ","
        << "\"depositConfirm\":" << f.depositConfirm << ","
        << "\"withdrawConfirm\":" << f.withdrawConfirm << ","
        << "\"minDepositAmount\":" << f.minDepositAmount << ","
        << "\"minWithdrawAmount\":" << f.minWithdrawAmount << ","
        << "\"browserUrl\":" << f.browserUrl << "}";

    return os;
}

currencies_t::currency_t currencies_t::currency_t::construct(simdjson::ondemand::object &obj) {
    currency_t res = {};
    for (auto field : obj) {
        if (field.value().is_null()) continue;
        std::string_view key = field.unescaped_key();
        if (key == "coinId") {
            res.coinId = field.value().get_uint64_in_string();
        } else if (key == "coinName") {
            res.coinName = field.value().get_string();
        } else if (key == "transfer") {
            std::string_view value = field.value().get_string();
            const char* val = std::string(value).c_str();
            res.transfer = val == "true";
        } else if (key == "chains") {
            simdjson::ondemand::array chains(field.value().get_array());
            for (auto x : chains) {
                res.chains.emplace_back(chain_t::construct(x.get_object().value()));
            }
        }
    }
    return res;
}

std::ostream &operator<<(std::ostream &os, const currencies_t::currency_t &f) {
    os << "{"
        << "\"coinId\":" << f.coinId << ","
        << "\"coinName\":" << f.coinName << ","
        << "\"transfer\":" << std::boolalpha << f.transfer << ","
        << "\"chains\":" << "[";
        for (auto it = f.chains.begin(); it != f.chains.end(); ++it) {
            os << *it;
            if (std::next(it) != f.chains.end())
                os << ",";
        }
    os << "]}";

    return os;
}

currencies_t currencies_t::construct(simdjson::ondemand::document &doc) {
    currencies_t res{};
    simdjson::ondemand::array currencies(doc["data"].get_array());
    for (auto x : currencies) {
        res.currencies.emplace_back(currency_t::construct(x.get_object().value()));
    }

    return res;
}

std::ostream &operator<<(std::ostream &os, const currencies_t &f) {
    os << "{"
        << "\"currencies\":" << "[";
        for (auto it = f.currencies.begin(); it != f.currencies.end(); ++it) {
            os << *it;
            if (std::next(it) != f.currencies.end())
                os << ",";
        }
    os << "]}";

    return os;
}

//------------------------------------------------------------------------------

product_t product_t::construct(simdjson::ondemand::document &doc) {
    simdjson::ondemand::object obj = doc["data"].get_object();
    return construct(obj);
}

product_t product_t::construct(simdjson::ondemand::object &obj) {
    product_t res = {};
    for (auto field : obj) {
        if (field.value().is_null()) continue;
        std::string_view key = field.unescaped_key();
        if (key == "symbol") {
            res.symbol = field.value().get_string();
        } else if (key == "symbolName") {
            res.symbolName = field.value().get_string();
        } else if (key == "baseCoin") {
            res.baseCoin = field.value().get_string();
        } else if (key == "quoteCoin") {
            res.quoteCoin = field.value().get_string();
        } else if (key == "minTradeAmount") {
            double_type minTradeAmount(static_cast<std::string>(field.value().get_string().value()));
            res.minTradeAmount = minTradeAmount;
        } else if (key == "maxTradeAmount") {
            double_type maxTradeAmount(static_cast<std::string>(field.value().get_string().value()));
            res.maxTradeAmount = maxTradeAmount;
        } else if (key == "takerFeeRate") {
            double_type takerFeeRate(static_cast<std::string>(field.value().get_string().value()));
            res.takerFeeRate = takerFeeRate;
        } else if (key == "makerFeeRate") {
            double_type makerFeeRate(static_cast<std::string>(field.value().get_string().value()));
            res.makerFeeRate = makerFeeRate;
        } else if (key == "priceScale") {
            res.priceScale = field.value().get_uint64_in_string();
        } else if (key == "quantityScale") {
            res.quantityScale = field.value().get_uint64_in_string();
        } else if (key == "status") {
            res.status = field.value().get_string();
        } else if (key == "minTradeUSDT") {
            res.minTradeUSDT = field.value().get_uint64_in_string();
        }
    }

    return res;
}

std::ostream &operator<<(std::ostream &os, const product_t &f) {
    os << "{"
        << "\"symbol\":" << f.symbol << ","
        << "\"symbolName\":" << f.symbolName << ","
        << "\"baseCoin\":" << f.baseCoin << ","
        << "\"quoteCoin\":" << f.quoteCoin << ","
        << "\"minTradeAmount\":" << f.minTradeAmount << ","
        << "\"maxTradeAmount\":" << f.maxTradeAmount << ","
        << "\"takerFeeRate\":" << f.takerFeeRate << ","
        << "\"makerFeeRate\":" << f.makerFeeRate << ","
        << "\"priceScale\":" << f.priceScale << ","
        << "\"quantityScale\":" << f.quantityScale << ","
        << "\"status\":" << f.status << ","
        << "\"minTradeUSDT\":" << f.minTradeUSDT << "}";

    return os;
}

products_t products_t::construct(simdjson::ondemand::document &doc) {
    products_t res{};
    simdjson::ondemand::array products(doc["data"].get_array());
    for (auto x : products) {
        res.products.emplace_back(product_t::construct(x.get_object().value()));
    }

    return res;
}

std::ostream &operator<<(std::ostream &os, const products_t &f) {
    os << "{"
        << "\"products\":" << "[";
        for (auto it = f.products.begin(); it != f.products.end(); ++it) {
            os << *it;
            if (std::next(it) != f.products.end())
                os << ",";
        }
    os << "]}";

    return os;
}

//------------------------------------------------------------------------------

spot_ticker_t spot_ticker_t::construct(simdjson::ondemand::document &doc) {
    simdjson::ondemand::object obj = doc["data"].get_object();
    return construct(obj);
}

spot_ticker_t spot_ticker_t::construct(simdjson::ondemand::object &obj) {
    spot_ticker_t res = {};
    for (auto field : obj) {
        if (field.value().is_null()) continue;
        std::string_view key = field.unescaped_key();
        if (key == "symbol") {
            res.symbol = field.value().get_string();
        } else if (key == "high24h") {
            double_type high24h(static_cast<std::string>(field.value().get_string().value()));
            res.high24h = high24h;
        } else if (key == "low24h") {
            double_type low24h(static_cast<std::string>(field.value().get_string().value()));
            res.low24h = low24h;
        } else if (key == "close") {
            double_type close(static_cast<std::string>(field.value().get_string().value()));
            res.close = close;
        } else if (key == "quoteVol") {
            double_type quoteVol(static_cast<std::string>(field.value().get_string().value()));
            res.quoteVol = quoteVol;
        } else if (key == "baseVol") {
            double_type baseVol(static_cast<std::string>(field.value().get_string().value()));
            res.baseVol = baseVol;
        } else if (key == "usdtVol") {
            double_type usdtVol(static_cast<std::string>(field.value().get_string().value()));
            res.usdtVol = usdtVol;
        } else if (key == "ts") {
            res.ts = field.value().get_uint64_in_string();
        } else if (key == "buyOne") {
            double_type buyOne(static_cast<std::string>(field.value().get_string().value()));
            res.buyOne = buyOne;
        } else if (key == "sellOne") {
            double_type sellOne(static_cast<std::string>(field.value().get_string().value()));
            res.sellOne = sellOne;
        } else if (key == "bidSz") {
            double_type bidSz(static_cast<std::string>(field.value().get_string().value()));
            res.bidSz = bidSz;
        } else if (key == "askSz") {
            double_type askSz(static_cast<std::string>(field.value().get_string().value()));
            res.askSz = askSz;
        } else if (key == "openUtc0") {
            double_type openUtc0(static_cast<std::string>(field.value().get_string().value()));
            res.openUtc0 = openUtc0;
        } else if (key == "changeUtc") {
            double_type changeUtc(static_cast<std::string>(field.value().get_string().value()));
            res.changeUtc = changeUtc;
        } else if (key == "change") {
            double_type change(static_cast<std::string>(field.value().get_string().value()));
            res.change = change;
        }
    }

    return res;
}

std::ostream &operator<<(std::ostream &os, const spot_ticker_t &f) {
    os << "{"
        << "\"symbol\":" << f.symbol << ","
        << "\"high24h\":" << f.high24h << ","
        << "\"low24h\":" << f.low24h << ","
        << "\"close\":" << f.close << ","
        << "\"quoteVol\":" << f.quoteVol << ","
        << "\"baseVol\":" << f.baseVol << ","
        << "\"usdtVol\":" << f.usdtVol << ","
        << "\"ts\":" << f.ts << ","
        << "\"buyOne\":" << f.buyOne << ","
        << "\"sellOne\":" << f.sellOne << ","
        << "\"bidSz\":" << f.bidSz << ","
        << "\"askSz\":" << f.askSz << ","
        << "\"openUtc0\":" << f.openUtc0 << ","
        << "\"changeUtc\":" << f.changeUtc << ","
        << "\"change\":" << f.change << "}";

    return os;
}

spot_tickers_t spot_tickers_t::construct(simdjson::ondemand::document &doc) {
    spot_tickers_t res{};
    simdjson::ondemand::array tickers(doc["data"].get_array());
    for (auto x : tickers) {
        res.tickers.emplace_back(spot_ticker_t::construct(x.get_object().value()));
    }

    return res;
}

std::ostream &operator<<(std::ostream &os, const spot_tickers_t &f) {
    os << "{"
        << "\"tickers\":" << "[";
        for (auto it = f.tickers.begin(); it != f.tickers.end(); ++it) {
            os << *it;
            if (std::next(it) != f.tickers.end())
                os << ",";
        }
    os << "]}";

    return os;
}

//------------------------------------------------------------------------------

spot_fills_t::spot_fill_t spot_fills_t::spot_fill_t::construct(simdjson::ondemand::object &obj) {
    spot_fill_t res = {};
    for (auto field : obj) {
        if (field.value().is_null()) continue;
        std::string_view key = field.unescaped_key();
        if (key == "symbol") {
            res.symbol = field.value().get_string();
        } else if (key == "tradeId") {
            res.tradeId = field.value().get_string();
        } else if (key == "side") {
            res.side = side_from_string(field.value().get_string());
        } else if (key == "fillPrice") {
            double_type fillPrice(static_cast<std::string>(field.value().get_string().value()));
            res.fillPrice = fillPrice;
        } else if (key == "fillQuantity") {
            double_type fillQuantity(static_cast<std::string>(field.value().get_string().value()));
            res.fillQuantity = fillQuantity;
        } else if (key == "fillTime") {
            res.fillTime = field.value().get_uint64_in_string();
        }
    }

    return res;
}

std::ostream &operator<<(std::ostream &os, const spot_fills_t::spot_fill_t &f) {
    os << "{"
        << "\"symbol\":" << f.symbol << ","
        << "\"tradeId\":" << f.tradeId << ","
        << "\"side\":" << side_to_string(f.side) << ","
        << "\"fillPrice\":" << f.fillPrice << ","
        << "\"fillQuantity\":" << f.fillQuantity << ","
        << "\"fillTime\":" << f.fillTime << "}";

    return os;
}

spot_fills_t spot_fills_t::construct(simdjson::ondemand::document &doc) {
    spot_fills_t res{};
    simdjson::ondemand::array fills(doc["data"].get_array());
    for (auto x : fills) {
        res.fills.emplace_back(spot_fill_t::construct(x.get_object().value()));
    }

    return res;
}

std::ostream &operator<<(std::ostream &os, const spot_fills_t &f) {
    os << "{"
        << "\"fills\":" << "[";
        for (auto it = f.fills.begin(); it != f.fills.end(); ++it) {
            os << *it;
            if (std::next(it) != f.fills.end())
                os << ",";
        }
    os << "]}";

    return os;
}

//------------------------------------------------------------------------------

spot_candles_t::spot_candle_t spot_candles_t::spot_candle_t::construct(simdjson::ondemand::object &obj) {
    spot_candle_t res = {};
    for (auto field : obj) {
        if (field.value().is_null()) continue;
        std::string_view key = field.unescaped_key();
        if (key == "open") {
            double_type open(static_cast<std::string>(field.value().get_string().value()));
            res.open = open;
        } else if (key == "high") {
            double_type high(static_cast<std::string>(field.value().get_string().value()));
            res.high = high;
        } else if (key == "low") {
            double_type low(static_cast<std::string>(field.value().get_string().value()));
            res.low = low;
        } else if (key == "close") {
            double_type close(static_cast<std::string>(field.value().get_string().value()));
            res.close = close;
        } else if (key == "quoteVol") {
            double_type quoteVol(static_cast<std::string>(field.value().get_string().value()));
            res.quoteVol = quoteVol;
        } else if (key == "baseVol") {
            double_type baseVol(static_cast<std::string>(field.value().get_string().value()));
            res.baseVol = baseVol;
        } else if (key == "usdtVol") {
            double_type usdtVol(static_cast<std::string>(field.value().get_string().value()));
            res.usdtVol = usdtVol;
        } else if (key == "ts") {
            res.ts = field.value().get_uint64_in_string();
        }
    }

    return res;
}

std::ostream &operator<<(std::ostream &os, const spot_candles_t::spot_candle_t &f) {
    os << "{"
        << "\"open\":" << f.open << ","
        << "\"high\":" << f.high << ","
        << "\"low\":" << f.low << ","
        << "\"close\":" << f.close << ","
        << "\"quoteVol\":" << f.quoteVol << ","
        << "\"baseVol\":" << f.baseVol << ","
        << "\"usdtVol\":" << f.usdtVol << ","
        << "\"ts\":" << f.ts << "}";

    return os;
}

spot_candles_t spot_candles_t::construct(simdjson::ondemand::document &doc) {
    spot_candles_t res{};
    simdjson::ondemand::array candles(doc["data"].get_array());
    for (auto x : candles) {
        res.candles.emplace_back(spot_candle_t::construct(x.get_object().value()));
    }

    return res;
}

std::ostream &operator<<(std::ostream &os, const spot_candles_t &f) {
    os << "{"
        << "\"candles\":" << "[";
        for (auto it = f.candles.begin(); it != f.candles.end(); ++it) {
            os << *it;
            if (std::next(it) != f.candles.end())
                os << ",";
        }
    os << "]}";

    return os;
}

//------------------------------------------------------------------------------

spot_depth_t spot_depth_t::construct(simdjson::ondemand::document &doc) {
    spot_depth_t res = {};
    simdjson::ondemand::object data(doc["data"].get_object());
    for (auto field : data) {
        if (field.value().is_null()) continue;
        std::string_view key = field.unescaped_key();
        if (key == "asks") {
            simdjson::ondemand::array asks(field.value().get_array());
            for (auto x : asks) {
                double_type level = double_type(static_cast<std::string>(x.at(0).get_string().value()));
                double_type amount = double_type(static_cast<std::string>(x.at(1).get_string().value()));
                res.asks.insert(level, amount);
            }
        } else if (key == "bids") {
            simdjson::ondemand::array bids(field.value().get_array());
            for (auto x : bids) {
                double_type level = double_type(static_cast<std::string>(x.at(0).get_string().value()));
                double_type amount = double_type(static_cast<std::string>(x.at(1).get_string().value()));
                res.bids.insert(level, amount);
            }
        } else if (key == "timestamp") {
            res.timestamp = field.value().get_uint64_in_string();
        }
    }

    return res;
}

std::ostream &operator<<(std::ostream &os, const spot_depth_t &f) {
    os << "{"
        << "\"asks\":[";
    for (auto it = f.asks.begin(); it != f.asks.end(); ++it) {
        os << "{" << it->first << ":" << it->second << "}";
        if (std::next(it) != f.asks.end())
            os << ",";
    }
    os << "],"
        << "\"bids\":[";
    for (auto it = f.bids.begin(); it != f.bids.end(); ++it) {
        os << "{" << it->first << ":" << it->second << "}";
        if (std::next(it) != f.bids.end())
            os << ",";
    }
    os << "],"
        << "\"timestamp\":" << f.timestamp << "}";

    return os;
}

//------------------------------------------------------------------------------

spot_vip_rates_t::rate_t spot_vip_rates_t::rate_t::construct(simdjson::ondemand::object &obj) {
    rate_t res = {};
    for (auto field : obj) {
        if (field.value().is_null()) continue;
        std::string_view key = field.unescaped_key();
        if (key == "level") {
            res.level = field.value().get_uint64_in_string();
        } else if (key == "dealAmount") {
            double_type dealAmount = double_type(static_cast<std::string>(field.value().get_string().value()));
            res.dealAmount = dealAmount;
        } else if (key == "assetAmount") {
            double_type assetAmount = double_type(static_cast<std::string>(field.value().get_string().value()));
            res.assetAmount = assetAmount;
        } else if (key == "makerFeeRate") {
            double_type makerFeeRate(static_cast<std::string>(field.value().get_string().value()));
            res.makerFeeRate = makerFeeRate;
        } else if (key == "takerFeeRate") {
            double_type takerFeeRate(static_cast<std::string>(field.value().get_string().value()));
            res.takerFeeRate = takerFeeRate;
        } else if (key == "withdrawAmount") {
            double_type withdrawAmount(static_cast<std::string>(field.value().get_string().value()));
            res.withdrawAmount = withdrawAmount;
        } else if (key == "withdrawAmountUSDT") {
            double_type withdrawAmountUSDT(static_cast<std::string>(field.value().get_string().value()));
            res.withdrawAmountUSDT = withdrawAmountUSDT;
        }
    }

    return res;
}

std::ostream &operator<<(std::ostream &os, const spot_vip_rates_t::rate_t &f) {
    os << "{"
        << "\"level\":" << f.level << ","
        << "\"dealAmount\":" << f.dealAmount << ","
        << "\"assetAmount\":" << f.assetAmount << ","
        << "\"makerFeeRate\":" << f.makerFeeRate << ","
        << "\"takerFeeRate\":" << f.takerFeeRate << ","
        << "\"withdrawAmount\":" << f.withdrawAmount << ","
        << "\"withdrawAmountUSDT\":" << f.withdrawAmountUSDT << "}";

    return os;
}

spot_vip_rates_t spot_vip_rates_t::construct(simdjson::ondemand::document &doc) {
    spot_vip_rates_t res = {};
    simdjson::ondemand::array rates(doc["data"].get_array());
    for (auto x : rates) {
        res.rates.emplace_back(rate_t::construct(x.get_object().value()));
    }

    return res;
}

std::ostream &operator<<(std::ostream &os, const spot_vip_rates_t &f) {
    os << "{"
        << "\"rates\":[";
    for (auto it = f.rates.begin(); it != f.rates.end(); ++it) {
        os << *it;
        if (std::next(it) != f.rates.end())
            os << ",";
    }
    os << "]}";

    return os;
}

//------------------------------------------------------------------------------

static bool validate_transfer(_from_to_type fromType, _from_to_type toType, const char* coin) {
    switch (fnv1a(coin)) {
        case fnv1a("USDT"):
            return (fromType == _from_to_type::_spot && toType == _from_to_type::_spot)
                || (fromType == _from_to_type::_spot && toType == _from_to_type::_mix_usdt)
                || (fromType == _from_to_type::_mix_usdt && toType == _from_to_type::_spot)
                || (fromType == _from_to_type::_mix_usdt && toType == _from_to_type::_mix_usdt);
        case fnv1a("BTC"):
        case fnv1a("ETH"):
        case fnv1a("EOS"):
        case fnv1a("XRP"):
            return (fromType == _from_to_type::_spot && toType == _from_to_type::_spot)
                || (fromType == _from_to_type::_spot && toType == _from_to_type::_mix_usd)
                || (fromType == _from_to_type::_mix_usd && toType == _from_to_type::_spot)
                || (fromType == _from_to_type::_mix_usd && toType == _from_to_type::_mix_usd);
        case fnv1a("USDC"):
            return (fromType == _from_to_type::_spot && toType == _from_to_type::_spot)
                || (fromType == _from_to_type::_spot && toType == _from_to_type::_mix_usd)
                || (fromType == _from_to_type::_spot && toType == _from_to_type::_mix_usdc)
                || (fromType == _from_to_type::_mix_usd && toType == _from_to_type::_spot)
                || (fromType == _from_to_type::_mix_usd && toType == _from_to_type::_mix_usd)
                || (fromType == _from_to_type::_mix_usd && toType == _from_to_type::_mix_usdc)
                || (fromType == _from_to_type::_mix_usdc && toType == _from_to_type::_spot)
                || (fromType == _from_to_type::_mix_usdc && toType == _from_to_type::_mix_usd)
                || (fromType == _from_to_type::_mix_usdc && toType == _from_to_type::_mix_usdc);
        default:
            return fromType == _from_to_type::_spot && toType == _from_to_type::_spot;
    }
}

transfer_req_t transfer_req_t::construct(_from_to_type fromType, _from_to_type toType, double_type amount, const char* coin, const char* clientOid) {
    transfer_req_t res = {};
    if (validate_transfer(fromType, toType, coin)) {
        res.fromType = fromType;
        res.toType = toType;
        res.amount = amount;
        res.coin = coin;
        res.clientOid = clientOid;
    }

    return res;
}

std::ostream &operator<<(std::ostream &os, const transfer_req_t &f) {
    os << "{"
        << "\"fromType\":" << from_to_type_to_string(f.fromType) << ","
        << "\"toType\":" << from_to_type_to_string(f.toType) << ","
        << "\"amount\":" << f.amount << ","
        << "\"coin\":" << f.coin;
    if (strnlen_s(f.clientOid, 1) != 0)
        os << "," << "\"clientOid\":" << f.clientOid << "}";
    else 
        os << "}";

    return os;
}

transfer_res_t transfer_res_t::construct(simdjson::ondemand::document &doc) {
    transfer_res_t res = {};
    res.success = (doc["msg"].get_string().value() == "success");

    return res;
}

std::ostream &operator<<(std::ostream &os, const transfer_res_t &f) {
    os << "{" << "\"success\":" << std::boolalpha << f.success << "}";

    return os;
}

//------------------------------------------------------------------------------

deposit_address_t deposit_address_t::construct(simdjson::ondemand::document &doc) {
    deposit_address_t res = {};
    simdjson::ondemand::object obj = doc["data"].get_object();
    for (auto field : obj) {
        if (field.value().is_null()) continue;
        std::string_view key = field.unescaped_key();
        if (key == "address") {
            res.address = field.value().get_string();
        } else if (key == "chain") {
            res.chain = field.value().get_string();
        } else if (key == "coin") {
            res.coin = field.value().get_string();
        } else if (key == "tag") {
            res.tag = field.value().get_string();
        } else if (key == "url") {
            res.url = field.value().get_string();
        }
    }

    return res;
}

std::ostream &operator<<(std::ostream &os, const deposit_address_t &f) {
    os << "{"
        << "\"address\":" << f.address << ","
        << "\"chain\":" << f.chain << ","
        << "\"coin\":" << f.coin << ","
        << "\"tag\":" << f.tag << ","
        << "\"url\":" << f.url
        << "}";

    return os;
}

//------------------------------------------------------------------------------

withdraw_req_t withdraw_req_t::construct(const char* coin, const char* address, const char* chain, double_type amount, const char* tag, const char* remark, const char* clientOid) {
    withdraw_req_t res = {};
    res.coin = coin;
    res.address = address;
    res.chain = chain;
    res.amount = amount;
    res.tag = tag;
    res.remark = remark;
    res.clientOid = clientOid;

    return res;
}

std::ostream &operator<<(std::ostream &os, const withdraw_req_t &f) {
    os << "{"
        << "\"coin\":" << f.coin << ","
        << "\"address\":" << f.address << ","
        << "\"chain\":" << f.chain << ","
        << "\"amount\":" << f.amount << ","
        << "\"tag\":" << f.tag << ","
        << "\"remark\":" << f.remark << ",";
    if (strnlen_s(f.clientOid, 1) != 0)
        os << "," << "\"clientOid\":" << f.clientOid << "}";
    else 
        os << "}";

    return os;
}

inner_withdraw_req_t inner_withdraw_req_t::construct(const char* coin, const char* toUid, double_type amount, const char* clientOid) {
    inner_withdraw_req_t res = {};
    res.coin = coin;
    res.toUid = toUid;
    res.amount = amount;
    res.clientOid = clientOid;

    return res;
}

std::ostream &operator<<(std::ostream &os, const inner_withdraw_req_t &f) {
    os << "{"
        << "\"coin\":" << f.coin << ","
        << "\"toUid\":" << f.toUid << ","
        << "\"amount\":" << f.amount << ",";
    if (strnlen_s(f.clientOid, 1) != 0)
        os << "," << "\"clientOid\":" << f.clientOid << "}";
    else 
        os << "}";

    return os;
}

withdraw_res_t withdraw_res_t::construct(simdjson::ondemand::document &doc) {
    withdraw_res_t res = {};
    res.success = (doc["msg"].get_string().value() == "success");
    res.orderId = doc["data"].get_string();

    return res;
}

std::ostream &operator<<(std::ostream &os, const withdraw_res_t &f) {
    os << "{"
        << "\"success\":" << std::boolalpha << f.success
        << "," << "\"orderID\":" << f.orderId
        << "}";

    return os;
}

//------------------------------------------------------------------------------

tx_list_t::tx_t tx_list_t::tx_t::construct(simdjson::ondemand::object &obj) {
    tx_t res = {};
    for (auto field : obj) {
        if (field.value().is_null()) continue;
        std::string_view key = field.unescaped_key();
        if (key == "id") {
            res.id = field.value().get_string();
        } else if (key == "txId") {
            res.txId = field.value().get_string();
        } else if (key == "coin") {
            res.coin = field.value().get_string();
        } else if (key == "type") {
            res.type = field.value().get_string();
        } else if (key == "amount") {
            double_type amount = double_type(static_cast<std::string>(field.value().get_string().value()));
            res.amount = amount;
        } else if (key == "status") {
            res.status = field.value().get_string();
        } else if (key == "toAddress") {
            res.toAddress = field.value().get_string();
        } else if (key == "fee") {
            double_type fee = double_type(static_cast<std::string>(field.value().get_string().value()));
            res.fee = fee;
        } else if (key == "chain") {
            res.chain = field.value().get_string();
        } else if (key == "confirm") {
            res.confirm = field.value().get_uint64_in_string();
        } else if (key == "cTime") {
            res.cTime = field.value().get_uint64_in_string();
        } else if (key == "uTime") {
            res.uTime = field.value().get_uint64_in_string();
        }
    }

    return res;
}

std::ostream &operator<<(std::ostream &os, const tx_list_t::tx_t &f) {
    os << "{"
        << "\"id\":" << f.id << ","
        << "\"txId\":" << f.txId << ","
        << "\"coin\":" << f.coin << ","
        << "\"type\":" << f.type << ","
        << "\"amount\":" << f.amount << ","
        << "\"status\":" << f.status << ","
        << "\"toAddress\":" << f.toAddress << ","
        << "\"fee\":" << f.fee << ","
        << "\"chain\":" << f.chain << ","
        << "\"confirm\":" << f.confirm << ","
        << "\"cTime\":" << f.cTime << ","
        << "\"uTime\":" << f.uTime
        << "}";

    return os;
}

tx_list_t tx_list_t::construct(simdjson::ondemand::document &doc) {
    tx_list_t res = {};
    simdjson::ondemand::array transactions(doc["data"].get_array());
    for (auto x : transactions) {
        res.transactions.emplace_back(tx_t::construct(x.get_object().value()));
    }

    return res;
}

std::ostream &operator<<(std::ostream &os, const tx_list_t &f) {
    os << "{" << "\"withdrawals\": [";
    for (auto it = f.transactions.begin(); it != f.transactions.end(); ++it) {
        os << *it;
        if (std::next(it) != f.transactions.end())
            os << ",";
    }
    os << "]}";

    return os;
}

//------------------------------------------------------------------------------

apikey_t apikey_t::construct(simdjson::ondemand::document &doc) {
    apikey_t res = {};
    simdjson::ondemand::object data(doc["data"].get_object());
    for (auto field : data) {
        if (field.value().is_null()) continue;
        std::string_view key = field.unescaped_key();
        if (key == "user_id") {
            res.user_id = field.value().get_uint64_in_string();
        } else if (key == "inviter_id") {
            res.inviter_id = field.value().get_string();
        } else if (key == "agent_inviter_code") {
            res.agent_inviter_code = field.value().get_string();
        } else if (key == "channel") {
            res.channel = field.value().get_string();
        } else if (key == "ips") {
            res.ips = field.value().get_string();
        } else if (key == "authorities") {
            simdjson::ondemand::array authorities(field.value().get_array());
            for (auto x : authorities) {
                res.auths.emplace_back(x.get_string().value());
            }
        } else if (key == "parentId") {
            res.parentId = field.value().get_uint64();
        } else if (key == "trader") {
            res.trader = field.value().get_bool();
        }
    }

    return res;
}

std::ostream &operator<<(std::ostream &os, const apikey_t &f) {
    os << "{"
        << "\"user_id\":" << f.user_id << ","
        << "\"inviter_id\":" << f.inviter_id << ","
        << "\"agent_inviter_code\":" << f.agent_inviter_code << ","
        << "\"channel\":" << f.channel << ","
        << "\"ips\":" << f.ips << ","
        << "\"authorities\":";
    for (auto it = f.auths.begin(); it != f.auths.end(); ++it) {
        os << *it;
        if (std::next(it) != f.auths.end())
            os << ",";
    }
    os << "\"parentId\":" << f.parentId << ","
        << "\"trader\":" << std::boolalpha << f.trader
        << "}";

    return os;
}

//------------------------------------------------------------------------------

assets_t::asset_t assets_t::asset_t::construct(simdjson::ondemand::object &obj) {
    asset_t res = {};
    for (auto field : obj) {
        if (field.value().is_null()) continue;
        std::string_view key = field.unescaped_key();
        if (key == "coinId") {
            res.coinId = field.value().get_uint64();
        } else if (key == "coinName") {
            res.coinName = field.value().get_string();
        } else if (key == "available") {
            double_type available = double_type(static_cast<std::string>(field.value().get_string().value()));
            res.available = available;
        } else if (key == "frozen") {
            double_type frozen = double_type(static_cast<std::string>(field.value().get_string().value()));
            res.frozen = frozen;
        } else if (key == "lock") {
            double_type lock = double_type(static_cast<std::string>(field.value().get_string().value()));
            res.lock = lock;
        } else if (key == "uTime") {
            res.uTime = field.value().get_uint64_in_string();
        }
    }

    return res;
}

std::ostream &operator<<(std::ostream &os, const assets_t::asset_t &f) {
    os << "{"
        << "\"coinId\":" << f.coinId << ","
        << "\"coinName\":" << f.coinName << ","
        << "\"available\":" << f.available << ","
        << "\"frozen\":" << f.frozen << ","
        << "\"lock\":" << f.lock << ","
        << "\"uTime\":" << f.uTime
        << "}";

    return os;
}

assets_t assets_t::construct(simdjson::ondemand::document &doc) {
    assets_t res = {};
    simdjson::ondemand::array assets(doc["data"].get_array());
    for (auto x : assets) {
        res.assets.emplace_back(asset_t::construct(x.get_object().value()));
    }

    return res;
}

std::ostream &operator<<(std::ostream &os, const assets_t &f) {
    os << "{" << "\"assets\": [";
    for (auto it = f.assets.begin(); it != f.assets.end(); ++it) {
        os << *it;
        if (std::next(it) != f.assets.end())
            os << ",";
    }
    os << "]}";

    return os;
}

//------------------------------------------------------------------------------

} // ns rest

namespace ws {

//------------------------------------------------------------------------------



} // ns ws

} // ns bg_api