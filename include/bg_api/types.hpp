
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
    
    // Spot endpoints

    struct server_time_t {
        time_t requestTime;
        time_t serverTime;
        
        static server_time_t construct(simdjson::ondemand::document &doc);
        friend std::ostream &operator<<(std::ostream &os, const server_time_t &f);
    };

    struct coin_t {
        std::size_t coinId;
        std::string_view coinName;
        bool transfer;

        struct chain_t {
            std::string_view chain;
            bool needTag;
            bool withdrawable;
            bool rechargeable;
            double_type withdrawFee;
            double_type extraWithDrawFee;
            std::size_t depositConfirm;
            std::size_t withdrawConfirm;
            double_type minDepositAmount;
            double_type minWithdrawAmount;
            std::string_view browserUrl;

            static chain_t construct(simdjson::ondemand::object &obj);
            friend std::ostream &operator<<(std::ostream &os, const chain_t &f);
        };
        std::vector<chain_t> chains;

        static coin_t construct(simdjson::ondemand::object &obj);
        friend std::ostream &operator<<(std::ostream &os, const coin_t &f);
    };

    struct coin_list_t {
        std::vector<coin_t> coins;

        static coin_list_t construct(simdjson::ondemand::document &doc);
        friend std::ostream &operator<<(std::ostream &os, const coin_list_t &f);
    };

    struct symbol_t {
        std::string_view symbol;
        std::string_view symbolName;
        std::string_view baseCoin;
        std::string_view quoteCoin;
        double_type minTradeAmount;
        double_type maxTradeAmount;
        double_type takerFeeRate;
        double_type makerFeeRate;
        std::size_t priceScale;
        std::size_t quantityScale;
        std::string_view status;

        static symbol_t construct(simdjson::ondemand::document &doc);
        static symbol_t construct(simdjson::ondemand::object &obj);
        friend std::ostream &operator<<(std::ostream &os, const symbol_t &f);
    };

    struct symbols_t {
        std::vector<symbol_t> symbols;

        static symbols_t construct(simdjson::ondemand::document &doc);
        friend std::ostream &operator<<(std::ostream &os, const symbols_t &f);
    };
    
    struct spot_ticker_t {
        std::string_view symbol;
        double_type high24h;
        double_type low24h;
        double_type close;
        double_type quoteVol;
        double_type baseVol;
        double_type usdtVol;
        std::size_t ts;
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

    struct trade_t {
        std::string_view symbol;
        std::string_view tradeId;
        _side side;
        double_type fillPrice;
        double_type fillQuantity;
        std::size_t fillTime;

        //static trade_t construct(simdjson::ondemand::document &doc);
        static trade_t construct(simdjson::ondemand::object &obj);
        friend std::ostream &operator<<(std::ostream &os, const trade_t &f);
    };
    
    struct trades_t {
        std::vector<trade_t> trades;

        static trades_t construct(simdjson::ondemand::document &doc);
        friend std::ostream &operator<<(std::ostream &os, const trades_t &f);
    };

    struct candle_t {
        double_type open;
        double_type high;
        double_type low;
        double_type close;
        double_type quoteVol;
        double_type baseVol;
        std::size_t ts;

        static candle_t construct(simdjson::ondemand::object &obj);
        static candle_t construct(simdjson::ondemand::array &arr);
        friend std::ostream &operator<<(std::ostream &os, const candle_t &f);
    };

    struct candles_t {
        std::vector<candle_t> candles;

        static candles_t construct(simdjson::ondemand::document &doc);
        friend std::ostream &operator<<(std::ostream &os, const candles_t &f);
    };
    
    struct depth_t {
        std::map<double_type, double_type> asks;
        std::map<double_type, double_type> bids;
        std::size_t timestamp;

        static depth_t construct(simdjson::ondemand::document &doc);
        friend std::ostream &operator<<(std::ostream &os, const depth_t &f);
    };

    struct transfer_res_t {
        std::string_view msg;

        static transfer_res_t construct(simdjson::ondemand::document &doc);
        friend std::ostream &operator<<(std::ostream &os, const transfer_res_t &f);
    };

    struct address_t {
        std::string_view address;
        std::string_view chain;
        std::string_view coin;
        std::string_view tag;
        std::string_view url;

        static address_t construct(simdjson::ondemand::document &doc);
        friend std::ostream &operator<<(std::ostream &os, const address_t &f);
    };
    
    struct withdraw_res_t {
        std::string_view data;

        static withdraw_res_t construct(simdjson::ondemand::document &doc);
        friend std::ostream &operator<<(std::ostream &os, const withdraw_res_t &f);
    };

    struct deposit_withdrawal_t {
        std::string_view id;
        std::string_view txId;
        std::string_view coin;
        std::string_view type;
        double_type amount;
        std::string_view status;
        std::string_view toAddress;
        std::string_view chain;
        std::size_t cTime;
        std::size_t uTime;

        static deposit_withdrawal_t construct(simdjson::ondemand::document &doc);
        friend std::ostream &operator<<(std::ostream &os, const deposit_withdrawal_t &f);
    };

    struct apikey_t {
        std::string_view userId;
        std::string_view inviterId;
        std::string_view ips;
        std::set<std::string> auths;
        std::string_view parentId;
        std::string_view trader;

        static apikey_t construct(const simdjson::padded_string json);
        friend std::ostream &operator<<(std::ostream &os, const apikey_t &f);
    };

    struct spot_account_t {
        std::size_t coinId;
        std::string_view coinName;
        double_type available;
        double_type lock;
        std::size_t uTime;

        static spot_account_t construct(const simdjson::padded_string json);
        friend std::ostream &operator<<(std::ostream &os, const spot_account_t &f);
    };

    struct bill_t {
        std::size_t cTime;
        std::size_t coinId;
        std::string_view coinName;
        _group_type groupType;
        _biz_type bizType;
        double_type quantity;
        double_type balance;
        double_type fees;
        std::size_t billId;

        static bill_t construct(const simdjson::padded_string json);
        friend std::ostream &operator<<(std::ostream &os, const bill_t &f);
    };

    struct transfer_t {
        std::string_view coinName;
        std::string_view status;
        std::string_view toType;
        std::string_view toSymbol;
        std::string_view fromType;
        std::string_view fromSymbol;
        double_type amount;
        std::size_t tradeTime;

        static transfer_t construct(const simdjson::padded_string json);
        friend std::ostream &operator<<(std::ostream &os, const transfer_t &f);
    };

    struct spot_order_t {
        std::size_t accountId;
        std::string_view symbol;
        std::string_view orderId;
        std::string_view clientOrderId;
        double_type price;
        double_type quantity;
        _order_type orderType;
        _side side;
        _status status;
        double_type fillPrice;
        double_type fillQuantity;
        double_type fillTotalAmount;
        std::size_t cTime;

        static spot_order_t construct(const simdjson::padded_string json);
        friend std::ostream &operator<<(std::ostream &os, const spot_order_t &f);
    };

    struct transaction_t {
        std::size_t accountId;
        std::string_view symbol;
        std::string_view orderId;
        std::string_view fillId;
        _order_type orderType;
        _side side;
        double_type fillPrice;
        double_type fillQuantity;
        double_type fillTotalAmount;
        std::size_t cTime;
        std::string_view feeCcy;
        double_type fees;

        static transaction_t construct(const simdjson::padded_string json);
        friend std::ostream &operator<<(std::ostream &os, const transaction_t &f);
    };

    struct spot_plan_order_t {
        std::string_view orderId;
        std::string_view symbol;
        double_type size;
        double_type executePrice;
        double_type triggerPrice;
        _plan_status status;
        _order_type orderType;
        _side side;
        _trigger_type triggerType;
        std::size_t cTime;

        static spot_plan_order_t construct(const simdjson::padded_string json);
        friend std::ostream &operator<<(std::ostream &os, const spot_plan_order_t &f);
    };

//------------------------------------------------------------------------------

    // Futures endpoints

    struct contract_t {
        std::string_view baseCoin;
        std::string_view quoteCoin;
        std::string_view symbol;
        double_type buyLimitPriceRatio;
        double_type sellLimitPriceRatio;
        double_type feeRateUpRatio;
        double_type openCostUpRatio;
        double_type sizeMultiplier;
        double_type makerFeeRate;
        double_type takerFeeRate;
        double_type minTradeNum;
        std::size_t priceEndStep;
        std::size_t pricePlace;
        std::size_t volumePlace;
        std::string_view symbolType;
        std::vector<std::string> supportMarginCoins;

        static contract_t construct(const simdjson::padded_string json);
        friend std::ostream &operator<<(std::ostream &os, const contract_t &f);
    };

    struct fill_t {
        std::string_view tradeId;
        double_type price;
        double_type size;
        _side side;
        std::size_t timestamp;
        std::string_view symbol;

        static fill_t construct(const simdjson::padded_string json);
        friend std::ostream &operator<<(std::ostream &os, const fill_t &f);
    };

    struct symbol_leverage_t {
        std::string_view symbol;
        std::size_t minLeverage;
        std::size_t maxLeverage;

        static symbol_leverage_t construct(const simdjson::padded_string json);
        friend std::ostream &operator<<(std::ostream &os, const symbol_leverage_t &f);
    };

    struct futures_account_t {
        std::string_view marginCoin;
        double_type locked;
        double_type available;
        double_type crossMaxAvailable;
        double_type fixedMaxAvailable;
        double_type maxTransferOut;
        double_type equity;
        double_type usdtEquity;
        double_type btcEquity;
        double_type crossRiskRate;
        std::size_t crossMarginLeverage;
        std::size_t fixedLongLeverage;
        std::size_t fixedShortLeverage;
        _margin_mode marginMode;
        _hold_mode holdMode;

        static futures_account_t construct(const simdjson::padded_string json);
        friend std::ostream &operator<<(std::ostream &os, const futures_account_t &f);
    };

    struct futures_accounts_t {
        std::string_view marginCoin;
        double_type locked;
        double_type available;
        double_type crossMaxAvailable;
        double_type fixedMaxAvailable;
        double_type maxTransferOut;
        double_type equity;
        double_type usdtEquity;
        double_type btcEquity;

        static futures_accounts_t construct(const simdjson::padded_string json);
        friend std::ostream &operator<<(std::ostream &os, const futures_accounts_t &f);
    };

    struct position_t {
        std::string_view marginCoin;
        std::string_view symbol;
        _hold_side holdSide;
        double_type openDelegateCount;
        double_type margin;
        double_type available;
        double_type locked;
        double_type total;
        std::size_t leverage;
        double_type achievedProfits;
        double_type averageOpenPrice;
        _margin_mode marginMode;
        _hold_mode holdMode;
        double_type unrealizedPL;
        double_type keepMarginRate;
        double_type marketPrice;
        std::size_t cTime;

        static position_t construct(const simdjson::padded_string json);
        friend std::ostream &operator<<(std::ostream &os, const position_t &f);
    };

    struct futures_bill_t {
        std::string_view id;
        std::string_view symbol;
        std::string_view marginCoin;
        double_type amount;
        double_type fee;
        double_type feeByCoupon;
        std::string_view feeCoin;
        _business business;
        std::size_t cTime;

        static futures_bill_t construct(const simdjson::padded_string json);
        friend std::ostream &operator<<(std::ostream &os, const futures_bill_t &f);
    };

    struct coin_leverage_t {
        std::string_view symbol;
        std::string_view marginCoin;
        std::size_t longLeverage;
        std::size_t shortLeverage;
        _margin_mode marginMode;

        static coin_leverage_t construct(const simdjson::padded_string json);
        friend std::ostream &operator<<(std::ostream &os, const coin_leverage_t &f);
    };

    struct futures_open_order_t {
        std::string_view symbol;
        double_type size;
        std::string_view orderId;
        std::string_view clientOid;
        double_type filledQty;
        double_type fee;
        double_type price;
        _status state;
        _trade_side side;
        _force timeInForce;
        double_type totalProfits;
        _hold_side posSide;
        std::string_view marginCoin;
        _order_type orderType;
        std::size_t cTime;

        static futures_open_order_t construct(const simdjson::padded_string json);
        friend std::ostream &operator<<(std::ostream &os, const futures_open_order_t &f);
    };

    struct futures_history_order_t {
        std::string_view symbol;
        double_type size;
        std::string_view orderId;
        std::string_view clientOid;
        double_type filledQty;
        double_type fee;
        double_type price;
        double_type priceAvg;
        _status state;
        _trade_side side;
        _force timeInForce;
        double_type totalProfits;
        _hold_side posSide;
        std::string_view marginCoin;
        double_type filledAmount;
        _order_type orderType;
        std::size_t leverage;
        _margin_mode marginMode;
        std::size_t cTime;
        std::size_t uTime;

        static futures_history_order_t construct(const simdjson::padded_string json);
        friend std::ostream &operator<<(std::ostream &os, const futures_history_order_t &f);
    };

    struct futures_product_type_history_t {
        std::string_view symbol;
        double_type size;
        std::string_view orderId;
        std::string_view clientOid;
        double_type filledQty;
        double_type fee;
        double_type price;
        _status state;
        _trade_side side;
        _force timeInForce;
        double_type totalProfits;
        _hold_side posSide;
        std::string_view marginCoin;
        std::size_t leverage;
        _margin_mode marginMode;
        _order_type orderType;
        std::size_t cTime;

        static futures_product_type_history_t construct(const simdjson::padded_string json);
        friend std::ostream &operator<<(std::ostream &os, const futures_product_type_history_t &f);
    };

    struct futures_order_details_t {
        std::string_view symbol;
        double_type size;
        std::string_view orderId;
        std::string_view clientOid;
        double_type filledQty;
        double_type fee;
        double_type price;
        _status state;
        _trade_side side;
        _force timeInForce;
        double_type totalProfits;
        _hold_side posSide;
        std::string_view marginCoin;
        double_type presetTakeProfitPrice;
        double_type presetStopLossPrice;
        double_type filledAmount;
        _order_type orderType;
        std::size_t cTime;
        std::size_t uTime;

        static futures_order_details_t construct(const simdjson::padded_string json);
        friend std::ostream &operator<<(std::ostream &os, const futures_order_details_t &f);
    };

    struct futures_plan_order_t {
        std::string_view orderId;
        std::string_view symbol;
        std::string_view marginCoin;
        double_type size;
        double_type executePrice;
        double_type triggerPrice;
        _plan_status status;
        _order_type orderType;
        _plan_type planType;
        _trade_side side;
        _trigger_type triggerType;
        double_type presetTakeProfitPrice;
        double_type presetStopLossPrice;
        std::size_t cTime;

        static futures_plan_order_t construct(const simdjson::padded_string json);
        friend std::ostream &operator<<(std::ostream &os, const spot_plan_order_t &f);
    };

    struct futures_history_plan_order_t {
        std::string_view orderId;
        std::string_view executeOrderId;
        std::string_view symbol;
        std::string_view marginCoin;
        double_type size;
        double_type executePrice;
        double_type triggerPrice;
        _plan_status status;
        _order_type orderType;
        _plan_type planType;
        _trade_side side;
        _trigger_type triggerType;
        double_type presetTakeProfitPrice;
        double_type presetStopLossPrice;
        std::size_t cTime;

        static futures_history_plan_order_t construct(const simdjson::padded_string json);
        friend std::ostream &operator<<(std::ostream &os, const futures_history_plan_order_t &f);
    };

    struct futures_fill_t {
        std::string_view tradeId;
        std::string_view symbol;
        std::string_view orderId;
        double_type price;
        double_type sizeQty;
        double_type fee;
        _trade_side side;
        std::size_t cTime;

        static futures_fill_t construct(const simdjson::padded_string json);
        friend std::ostream &operator<<(std::ostream &os, const futures_fill_t &f);
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
