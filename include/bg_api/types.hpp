
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

#ifndef __bg_api__types_hpp
#define __bg_api__types_hpp

#include "double_type.hpp"
#include "enums.hpp"
#include "fnv1a.hpp"

#include <bg_api/simdjson.h>
#include <boost/variant.hpp>

#include <string>
#include <vector>
#include <map>
#include <set>
#include <cstdint>
#include <cassert>

// forward
namespace flatjson {
struct fjson;
} // ns flatjson

namespace bg_api {

/*************************************************************************************************/

namespace rest {

//------------------------------------------------------------------------------

    struct server_time_t {
        time_t serverTime;
        
        static server_time_t construct(simdjson::ondemand::document &doc);
        friend std::ostream &operator<<(std::ostream &os, const server_time_t &f);
    };

    struct currencies_t {
        struct currency_t {
            uint64_t coinId;
            std::string_view coinName;
            bool transfer;
            struct chain_t {
                std::string_view chain;
                bool needTag;
                bool withdrawable;
                bool rechargeable;
                double_type withdrawFee;
                double_type extraWithDrawFee;
                uint64_t depositConfirm;
                uint64_t withdrawConfirm;
                double_type minDepositAmount;
                double_type minWithdrawAmount;
                std::string_view browserUrl;

                static chain_t construct(simdjson::ondemand::object &obj);
                friend std::ostream &operator<<(std::ostream &os, const chain_t &f);
            };
            std::vector<chain_t> chains;

            static currency_t construct(simdjson::ondemand::object &obj);
            friend std::ostream &operator<<(std::ostream &os, const currency_t &f);
        };
        std::vector<currency_t> currencies;

        static currencies_t construct(simdjson::ondemand::document &doc);
        friend std::ostream &operator<<(std::ostream &os, const currencies_t &f);
    };

    struct product_t {
        std::string_view symbol;
        std::string_view symbolName;
        std::string_view baseCoin;
        std::string_view quoteCoin;
        double_type minTradeAmount;
        double_type maxTradeAmount;
        double_type takerFeeRate;
        double_type makerFeeRate;
        uint64_t priceScale;
        uint64_t quantityScale;
        std::string_view status;
        uint64_t minTradeUSDT;

        static product_t construct(simdjson::ondemand::document &doc);
        static product_t construct(simdjson::ondemand::object &obj);
        friend std::ostream &operator<<(std::ostream &os, const product_t &f);
    };

    struct products_t {
        std::vector<product_t> products;

        static products_t construct(simdjson::ondemand::document &doc);
        friend std::ostream &operator<<(std::ostream &os, const products_t &f);
    };

    struct spot_ticker_t {
        std::string_view symbol;
        double_type high24h;
        double_type low24h;
        double_type close;
        double_type quoteVol;
        double_type baseVol;
        double_type usdtVol;
        time_t ts;
        double_type buyOne;
        double_type sellOne;
        double_type bidSz;
        double_type askSz;
        double_type openUtc0;
        double_type changeUtc;
        double_type change;

        static spot_ticker_t construct(simdjson::ondemand::document &doc);
        static spot_ticker_t construct(simdjson::ondemand::object &obj);
        friend std::ostream &operator<<(std::ostream &os, const spot_ticker_t &f);
    };

    struct spot_tickers_t {
        std::vector<spot_ticker_t> tickers;

        static spot_tickers_t construct(simdjson::ondemand::document &doc);
        friend std::ostream &operator<<(std::ostream &os, const spot_tickers_t &f);
    };

    struct spot_fills_t {
        struct spot_fill_t {
            std::string_view symbol;
            std::string_view tradeId;
            _side side;
            double_type fillPrice;
            double_type fillQuantity;
            time_t fillTime;

            static spot_fill_t construct(simdjson::ondemand::object &obj);
            friend std::ostream &operator<<(std::ostream &os, const spot_fill_t &f);
        };
        std::vector<spot_fill_t> fills;

        static spot_fills_t construct(simdjson::ondemand::document &doc);
        friend std::ostream &operator<<(std::ostream &os, const spot_fills_t &f);
    };

    struct spot_candles_t {
        struct spot_candle_t {
            double_type open;
            double_type high;
            double_type low;
            double_type close;
            double_type quoteVol;
            double_type baseVol;
            double_type usdtVol;
            time_t ts;

            static spot_candle_t construct(simdjson::ondemand::object &obj);
            friend std::ostream &operator<<(std::ostream &os, const spot_candle_t &f);
        };
        std::vector<spot_candle_t> candles;

        static spot_candles_t construct(simdjson::ondemand::document &doc);
        friend std::ostream &operator<<(std::ostream &os, const spot_candles_t &f);
    };

    struct spot_depth_t {
        std::map<double_type, double_type> asks;
        std::map<double_type, double_type> bids;
        time_t timestamp;

        static spot_depth_t construct(simdjson::ondemand::document &doc);
        friend std::ostream &operator<<(std::ostream &os, const spot_depth_t &f);
    };

    struct spot_vip_rates_t {
        struct rate_t {
            uint64_t level;
            double_type dealAmount;
            double_type assetAmount;
            double_type takerFeeRate;
            double_type makerFeeRate;
            double_type withdrawAmount;
            double_type withdrawAmountUSDT;

            static rate_t construct(simdjson::ondemand::object &obj);
            friend std::ostream &operator<<(std::ostream &os, const rate_t &f);
        };
        std::vector<rate_t> rates;

        static spot_vip_rates_t construct(simdjson::ondemand::document &doc);
        friend std::ostream &operator<<(std::ostream &os, const spot_vip_rates_t &f);
    };

    struct transfer_req_t {
        _from_to_type fromType;
        _from_to_type toType;
        double_type amount;
        const char* coin;
        const char* clientOid;

        static transfer_req_t construct(_from_to_type fromType, _from_to_type toType, double_type amount, const char* coin, const char* clientOid = "");
        friend std::ostream &operator<<(std::ostream &os, const transfer_req_t &f);
    };

    struct transfer_res_t {
        bool success;

        static transfer_res_t construct(simdjson::ondemand::document &doc);
        friend std::ostream &operator<<(std::ostream &os, const transfer_res_t &f);
    };

    struct deposit_address_t {
        std::string_view address;
        std::string_view chain;
        std::string_view coin;
        std::string_view tag;
        std::string_view url;

        static deposit_address_t construct(simdjson::ondemand::document &doc);
        friend std::ostream &operator<<(std::ostream &os, const deposit_address_t &f);
    };

    struct withdraw_req_t {
        const char* coin;
        const char* address;
        const char* chain;
        double_type amount;
        const char* tag;
        const char* remark;
        const char* clientOid;

        static withdraw_req_t construct(const char* coin, const char* address, const char* chain, double_type amount, const char* tag = "", const char* remark = "", const char* clientOid = "");
        friend std::ostream &operator<<(std::ostream &os, const withdraw_req_t &f);
    };

    struct inner_withdraw_req_t {
        const char* coin;
        const char* toUid;
        double_type amount;
        const char* clientOid;

        static inner_withdraw_req_t construct(const char* coin, const char* toUid, double_type amount, const char* clientOid = "");
        friend std::ostream &operator<<(std::ostream &os, const inner_withdraw_req_t &f);
    };

    struct withdraw_res_t {
        bool success;
        std::string_view orderId;

        static withdraw_res_t construct(simdjson::ondemand::document &doc);
        friend std::ostream &operator<<(std::ostream &os, const withdraw_res_t &f);
    };

    struct tx_list_t {
        struct tx_t {
            std::string_view id;
            std::string_view txId;
            std::string_view coin;
            std::string_view type;
            double_type amount;
            std::string_view status;
            std::string_view toAddress;
            double_type fee;
            std::string_view chain;
            uint64_t confirm;
            time_t cTime;
            time_t uTime;

            static tx_t construct(simdjson::ondemand::object &obj);
            friend std::ostream &operator<<(std::ostream &os, const tx_t &f);
        };
        std::vector<tx_t> transactions;

        static tx_list_t construct(simdjson::ondemand::document &doc);
        friend std::ostream &operator<<(std::ostream &os, const tx_list_t &f);
    };

    struct apikey_t {
        uint64_t user_id;
        std::string_view inviter_id;
        std::string_view agent_inviter_code;
        std::string_view channel;
        std::string_view ips;
        std::vector<std::string> auths;
        uint64_t parentId;
        bool trader;

        static apikey_t construct(simdjson::ondemand::document &doc);
        friend std::ostream &operator<<(std::ostream &os, const apikey_t &f);
    };

    struct assets_t {
        struct asset_t {
            uint64_t coinId;
            std::string_view coinName;
            double_type available;
            double_type frozen;
            double_type lock;
            time_t uTime;

            static asset_t construct(simdjson::ondemand::object &obj);
            friend std::ostream &operator<<(std::ostream &os, const asset_t &f);
        };
        std::vector<asset_t> assets;

        static assets_t construct(simdjson::ondemand::document &doc);
        friend std::ostream &operator<<(std::ostream &os, const assets_t &f);
    };

    struct spot_bills_req_t {
        uint64_t coinId;
        _group_type groupType;
        _biz_type bizType;
        uint64_t before;
        uint64_t after;
        int limit;

        static spot_bills_req_t construct(uint64_t coinId, _group_type groupType = _group_type::_deposit, _biz_type bizType = _biz_type::_deposit, uint64_t before = 0, uint64_t after = 0, int limit = 100);
        friend std::ostream &operator<<(std::ostream &os, const spot_bills_req_t &f);
    };

    struct spot_bills_t {
        struct spot_bill_t {
            time_t cTime;
            

            static spot_bill_t construct(simdjson::ondemand::object &obj);
            friend std::ostream &operator<<(std::ostream &os, const spot_bill_t &f);
        };
        std::vector<spot_bill_t> bills;

        static spot_bills_t construct(simdjson::ondemand::document &doc);
        friend std::ostream &operator<<(std::ostream &os, const spot_bills_t &f);
    };

//------------------------------------------------------------------------------

} // ns rest

/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/

namespace ws {

//------------------------------------------------------------------------------

    // Public channels

    struct spot_ticker_t {
        std::string_view instId;
        double_type last;
        double_type bestAsk;
        double_type bestBid;
        double_type open24h;
        double_type high24h;
        double_type low24h;
        double_type baseVolume;
        double_type quoteVolume;
        std::size_t ts;
        std::size_t labelId;
        double_type openUtc;
        double_type chgUtc;
        double_type bidSz;
        double_type askSz;

        static spot_ticker_t construct(const simdjson::padded_string json);
        friend std::ostream &operator<<(std::ostream &os, const spot_ticker_t &f);
    };

    struct candle_t {
        std::size_t ts;
        double_type open;
        double_type high;
        double_type low;
        double_type close;
        double_type volume;

        static candle_t construct(const simdjson::padded_string json);
        friend std::ostream &operator<<(std::ostream &os, const candle_t &f);
    };

    struct depth_t {
        std::map<double_type, double_type> asks;
        std::map<double_type, double_type> bids;
        std::size_t ts;
        std::size_t checksum;

        static depth_t construct(const simdjson::padded_string json);
        friend std::ostream &operator<<(std::ostream &os, const depth_t &f);
    };

    struct trade_t {
        std::size_t ts;
        double_type price;
        double_type size;
        _side side;

        static trade_t construct(const simdjson::padded_string json);
        friend std::ostream &operator<<(std::ostream &os, const trade_t &f);
    };

    struct futures_ticker_t {
        std::string_view instId;
        double_type last;
        double_type high24h;
        double_type low24h;
        double_type bestBid;
        double_type bestAsk;
        double_type priceChangePct;
        double_type capitalRate;
        std::size_t nextSettleTime;
        std::size_t systemTime;
        double_type markPrice;
        double_type indexPrice;
        double_type holding;
        double_type baseVolume;
        double_type quoteVolume;
        double_type openUtc;
        double_type chgUtc;
        double_type bidSz;
        double_type askSz;

        static futures_ticker_t construct(const simdjson::padded_string json);
        friend std::ostream &operator<<(std::ostream &is, const futures_ticker_t &f);
    };

//------------------------------------------------------------------------------

} // ns ws

} // ns bg_api

#endif // __bg_api__types_hpp
