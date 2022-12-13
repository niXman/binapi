
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
    
    // Public endpoints

    struct server_time_t {
        std::size_t reqTime;
        std::size_t serverTime;
        
        static server_time_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const server_time_t &f);
    };

    struct coin_t {
        std::size_t coinId;
        std::string coinName;
        bool transfer;

        struct chain_t {
            std::string chain;
            bool needTag;
            bool withdrawAble;
            bool rechargeAble;
            double_type withdrawFee;
            std::size_t depositConfirm;
            std::size_t withdrawConfirm;
            double_type minDepositAmount;
            double_type minWithdrawAmount;
            std::string browseUrl;

            friend std::ostream &operator<<(std::ostream &os, const chain_t &f);
        };
        std::vector<chain_t> chainList;

        static coin_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const coin_t &f);
    };

    struct symbol_t {   // Single symbol
        std::string symbol;
        std::string symbolName;
        std::string baseCoin;
        std::string quoteCoin;
        double_type minTradeAmount;
        double_type maxTradeAmount;
        double_type takerFeeRate;
        double_type makerFeeRate;
        int priceScale;
        int quantityScale;

        static symbol_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const symbol_t &f);
    };

//------------------------------------------------------------------------------

    // Market endpoints

    struct ticker_t {
        std::string symbol;
        double_type last;            // Futures only. When called from spot, set to -1.0
        double_type bestAsk;         // Futures only. When called from spot, set to -1.0
        double_type bestBid;         // Futures only. When called from spot, set to -1.0
        double_type high24h;
        double_type low24h;
        double_type close;           // Spot only. When called from futures, set to -1.0
        double_type quoteVol;
        double_type baseVol;
        double_type usdtVol;
        std::size_t ts;
        double_type buyOne;
        double_type sellOne;
        double_type openUtc0;
        double_type priceChangePct;  // Futures only. When called from spot, set to -1.0
        double_type chgUtc;          // Futures only. When called from spot, set to -1.0

        static ticker_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const ticker_t &f);
    };

    struct trade_t {
        std::string symbol;
        std::string tradeId;
        _side side;
        double_type fillPrice;
        double_type fillQuantity;
        std::size_t fillTime;

        static trade_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const trade_t &f);
    };

    struct candle_t {
        double_type open;
        double_type high;
        double_type low;
        double_type close;
        double_type quoteVol;
        double_type baseVol;
        double_type usdtVol;
        std::size_t ts;

        static candle_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const candle_t &f);
    };

    struct depth_t {
        std::map<double_type, double_type> asks;
        std::map<double_type, double_type> bids;
        std::size_t timestamp;

        static depth_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const depth_t &f);
    };

    struct contract_t {
        std::string baseCoin;
        double_type buyLimitPriceRatio;
        double_type feeRateUpRatio;
        double_type makerFeeRate;
        double_type minTradeNum;
        double_type openCostUpRatio;
        int priceEndStep;
        int pricePlace;
        std::string quoteCoin;
        double_type sellLimitPriceRatio;
        double_type sizeMultiplier;
        std::vector<std::string> supportMarginCoins;
        std::string symbol;
        double_type takerFeeRate;
        int volumePlace;
        std::string symbolType;

        static contract_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const contract_t &f);
    };

    struct fill_t {
        std::string tradeId;
        double_type price;
        double_type size;
        _side side;
        std::size_t timestamp;
        std::string symbol;

        static fill_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const fill_t &f);
    };

    struct leverage_t {
        std::string symbol;
        int minLeverage;
        int maxLeverage;

        static leverage_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const leverage_t &f);
    };

//------------------------------------------------------------------------------

    // Wallet endpoints

    struct address_t {
        std::string address;
        std::string chain;
        std::string coin;
        std::string tag;
        std::string url;

        static address_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const address_t &f);
    };

    struct deposit_withdrawal_t {
        bool isDeposit;
        std::string id;
        std::string txId;
        std::string coin;
        std::string type;
        std::string amount;
        std::string status;
        std::string toAddress;
        double_type fee;         // Only for withdrawals. When called from getDeposit list set to -1.0
        std::string chain;
        int confirm;            // Only for withdrawals. When called from getDeposit list set to -1
        std::size_t cTime;
        std::size_t uTime;

        static deposit_withdrawal_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const deposit_withdrawal_t &f);
    };

//------------------------------------------------------------------------------

    // Account endpoints
    struct apikey_t {
        std::string userId;
        std::string inviterId;
        std::string ips;
        unsigned int auths;
        std::string parentId;
        bool trader;

        static apikey_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const apikey_t &f);
    };

    struct spot_account_t {
        std::size_t coinId;
        std::string coinName;
        int available;
        int lock;
        std::size_t uTime;

        static spot_account_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const spot_account_t &f);
    };

    struct bill_t {
        std::size_t cTime;
        std::size_t coinId;
        std::string coinName;
        _group_type groupType;
        _biz_type bizType;
        double_type quantity;
        double_type balance;
        double_type fees;
        std::size_t billId;

        static bill_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const bill_t &f);
    };

    struct transfer_t {
        std::string coinName;
        std::string status;
        std::string toType;
        std::string toSymbol;
        std::string fromType;
        std::string fromSymbol;
        double_type amount;
        std::size_t tradeTime;

        static transfer_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const transfer_t &f);
    };

    struct futures_account_t {
        std::string marginCoin;
        double_type locked;
        double_type available;
        double_type crossMaxAvailable;
        double_type fixedMaxAvailable;
        double_type maxTransferOut;
        double_type equity;
        double_type usdtEquity;
        double_type btcEquity;
        double_type crossRiskRate;
        int crossMarginLeverage;
        int fixedLongLeverage;
        int fixedShortLeverage;
        _margin_mode marginMode;
        _hold_mode holdMode;

        static futures_account_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const futures_account_t &f);
    };

    struct position_t {
        std::string marginCoin;
        std::string symbol;
        _hold_side holdSide;
        double_type openDelegateCount;
        double_type margin;
        double_type available;
        double_type locked;
        double_type total;
        int leverage;
        double_type achievedProfits;
        double_type averageOpenPrice;
        _margin_mode marginMode;
        _hold_mode holdMode;
        double_type unrealizedPL;
        double_type keepMarginRate;
        double_type marketPrice;
        std::size_t cTime;

        static position_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const position_t &f);
    };

    struct futures_bill_t {
        std::string id;
        std::string symbol;
        std::string marginCoin;
        double_type amount;
        double_type fee;
        double_type feeByCoupon;
        std::string feeCoin;
        _business business;
        std::size_t cTime;

        static futures_bill_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const futures_bill_t &f);
    };

    struct coin_leverage_t {
        std::string symbol;
        std::string marginCoin;
        int longLeverage;
        int shortLeverage;
        _margin_mode marginMode;

        static coin_leverage_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const coin_leverage_t &f);
    };

//------------------------------------------------------------------------------

    // Trade endpoints
    struct place_spot_order_t {
        _side side;
        _order_type orderType;
        _force force;
        double_type price;
        double_type quantity;
        std::string clientOrderId;

        static place_spot_order_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const place_spot_order_t &f);
    };

    struct place_futures_order_t {
        double_type size;
        double_type price;                   // Place order only. When called from placeReversal set to -1.0
        _mix_side side;
        _order_type orderType;
        _force force;
        std::string clientOid;
        double_type presetTP;                // Place order only. When called from placeReversal set to -1.0
        double_type presetSL;                // Place order only. When called from placeReversal set to -1.0
        bool reverse;                       // Reversal only. When called from placeOrder set to false

        static place_futures_order_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const place_futures_order_t &f);
    };

    struct spot_order_t {
        std::size_t accountId;
        std::string symbol;
        std::string orderId;
        std::string clientOrderId;
        double_type price;
        double_type quantity;
        _order_type orderType;
        _side side;
        _status status;
        double_type fillPrice;
        double_type fillQuantity;
        double_type fillTotalAmount;
        std::size_t cTime;

        static spot_order_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const spot_order_t &f);
    };

    struct futures_order_t {
        std::string symbol;
        double_type size;
        std::string orderId;
        std::string cliendOid;
        double_type filledQty;
        double_type fee;
        double_type price;
        double_type priceAvg;                    // history only
        _status state;
        _side side;
        _force timeInForce;
        double_type totalProfits;
        _hold_side posSide;
        std::string marginCoin;
        double_type presetTakeProfitPrice;       // details only
        double_type presetStopLossPrice;         // details only
        double_type filledAmount;                // history and details
        int leverage;                       // productType and history
        _order_type orderType;
        _margin_mode marginMode;     // productType and history
        std::size_t cTime;
        std::size_t uTime;                  // history and details

        static futures_order_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const futures_order_t &f);
    };

    struct transaction_t {
        std::size_t accountId;
        std::string symbol;
        std::string orderId;
        std::string fillId;
        _order_type orderType;
        _side side;
        double_type fillPrice;
        double_type fillQuantity;
        double_type fillTotalAmount;
        std::size_t cTime;
        std::string feeCcy;
        double_type fees;

        static transaction_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const transaction_t &f);
    };

    struct place_plan_order_spot_t {
        _side side;
        double_type triggerPrice;
        double_type executePrice;
        double_type size;
        _trigger_type triggerType;
        _order_type orderType;
        std::string clientOid;
        _force force;

        static place_plan_order_spot_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const place_plan_order_spot_t &f);
    };

    struct place_plan_order_futures_t {
        double_type size;
        double_type executePrice;
        double_type triggerPrice;
        _mix_side side;
        _order_type orderType;
        _force force;
        _trigger_type triggerType;
        std::string clientOid;
        double_type presetTP;
        double_type presetSL;

        static place_plan_order_futures_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const place_plan_order_futures_t &f);
    };

    struct spot_plan_order_t {
        std::string orderId;
        std::string executeOrderId;         // Futures history only
        std::string symbol;
        std::string marginCoin;             // Futures only
        double_type size;
        double_type executePrice;
        double_type triggerPrice;
        _plan_status status;
        _order_type orderType;
        _side side;
        _trigger_type triggerType;
        double_type presetTakeProfitPrice;       // Futures only
        double_type presetStopLossPrice;         // Futures only
        std::size_t cTime;

        static spot_plan_order_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const spot_plan_order_t &f);
    };

    struct futures_plan_order_t {
        std::string orderId;
        std::string executeOrderId;         // history only
        std::string symbol;
        std::string marginCoin;
        double_type size;
        double_type executePrice;
        double_type triggerPrice;
        _plan_status status;
        _order_type orderType;
        _mix_side side;
        _trigger_type triggerType;
        double_type presetTakeProfitPrice;
        double_type presetStopLossPrice;

        static futures_plan_order_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const spot_plan_order_t &f);
    };

    struct futures_fill_t {
        std::string tradeId;
        std::string symbol;
        std::string orderId;
        double_type price;
        double_type sizeQty;
        double_type fee;
        _trade_side side;
        std::size_t cTime;

        static futures_fill_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const futures_fill_t &f);
    };

//------------------------------------------------------------------------------

    // CopyTrade

    struct trader_order_t {
        std::string symbol;
        std::string trackingNo;
        std::string openOrderId;
        _hold_side holdSide;
        int openLeverage;
        double_type openAvgPrice;
        std::size_t openTime;
        double_type openDealCount;
        double_type stopProfitPrice;
        double_type stopLossPrice;
        double_type closeDealCount;
        std::size_t closeTime;      // History only
        double_type closeAvgPrice;       // History only
        _stop_type stopType;   // History only
        double_type achievedProfits;     // History only
        double_type openFee;             // History only
        double_type closeFee;            // History only

        static trader_order_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const trader_order_t &f);
    };

    struct follower_order_t {
        std::string trackingNo;
        std::string openOrderId;
        std::string closeOrderId;
        std::string symbol;
        _hold_side holdSide;
        double_type openSize;            // History only
        double_type closeSize;           // History only
        int openLeverage;
        double_type openAvgPrice;
        std::size_t openTime;
        double_type openDealCount;
        double_type openMargin;
        double_type avgClosePrice;
        double_type closeDealCount;
        std::size_t closeTime;
        _stop_type stopType;   // History only
        double_type achievedProfits;     // History only
        double_type openFee;             // History only
        double_type closeFee;            // History only
        double_type profitRate;          // History only
        double_type netProfit;           // History only

        static follower_order_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const follower_order_t &f);
    };

    struct trader_profit_t {
        double_type yesterdaySplitProfit;
        double_type sumProfit;
        double_type waitProfit;
        std::size_t yesterdayTimestamp;

        static trader_profit_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const trader_profit_t &f);
    };

    struct profit_token_t {
        std::string tokenId;
        double_type profit;
        std::size_t date;       // profitDateGroupList only
        std::string nickname;   // profitDateList only

        static profit_token_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const profit_token_t &f);
    };

    struct trader_symbols_t {
        std::string symbol;
        double_type minOpenCount;
        bool openTrader;

        static trader_symbols_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const trader_symbols_t &f);
    };

//------------------------------------------------------------------------------

    // Broker

    struct subaccount_t {
        std::string subUid;
        std::string subName;
        _account_status status;
        std::set<std::string> auths;
        std::string remark;
        std::size_t cTime;

        static subaccount_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const subaccount_t &f);
    };

    struct sub_spot_asset_t {
        std::size_t coinId;
        std::string coinName;
        double_type available;
        double_type frozen;
        double_type lock;
        std::size_t uTime;

        static sub_spot_asset_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const sub_spot_asset_t &f);
    };

    struct sub_futures_asset_t {
        std::string marginCoin;
        double_type locked;
        double_type available;
        double_type crossMaxAvailable;
        double_type fixedMaxAvailable;
        double_type maxTransferOut;
        double_type equity;
        double_type usdtEquity;
        double_type btcEquity;

        static sub_futures_asset_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const sub_futures_asset_t &f);
    };

    struct sub_address_t {
        std::string subUid;
        std::string address;
        std::string chain;
        std::string coin;
        std::string tag;
        std::string url;

        static sub_address_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const sub_address_t &f);
    };

    struct sub_apikey_t {
        std::string subUid;
        std::string label;
        std::string apiKey;
        unsigned int perms;
        std::string ip;

        static sub_apikey_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const sub_apikey_t &f);
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
        std::string instId;
        double_type last;
        double_type open24h;
        double_type high24h;
        double_type low24h;
        double_type bestBid;
        double_type bestAsk;
        double_type baseVolume;
        double_type quoteVolume;
        std::size_t ts;
        std::size_t labelId;
        double_type openUtc;
        double_type chgUtc;

        static spot_ticker_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const spot_ticker_t &f);
    };

    struct futures_ticker_t {
        std::string instId;
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

        static futures_ticker_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &is, const futures_ticker_t &f);
    };

    struct candle_t {
        std::size_t ts;
        double_type open;
        double_type high;
        double_type low;
        double_type close;
        double_type volume;

        static candle_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const candle_t &f);
    };

    struct depth_t {
        std::map<double_type, double_type> asks;
        std::map<double_type, double_type> bids;
        std::size_t ts;
        std::size_t checksum;

        static depth_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const depth_t &f);
    };

    struct trade_t {
        std::size_t ts;
        double_type price;
        double_type size;
        _side side;

        static trade_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const trade_t &f);
    };

//------------------------------------------------------------------------------

    // Private channels

    struct spot_account_t {
        struct coin_t {
            std::size_t coinId;
            std::string coinName;
            double_type available;

            friend std::ostream &operator<<(std::ostream &os, const coin_t &f);
        };
        std::vector<coin_t> coins;

        static spot_account_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const spot_account_t &f);
    };

    struct futures_account_t {
        std::string marginCoin;
        double_type locked;
        double_type available;
        double_type crossMaxAvailable;
        double_type fixedMaxAvailable;
        double_type maxTransferOut;
        double_type equity;
        double_type usdtEquity;

        static futures_account_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const futures_account_t &f);
    };

    struct position_t {
        std::string posId;
        std::string instId;
        std::string instName;
        std::string marginCoin;
        double_type margin;
        _margin_mode marginMode;
        _hold_side holdSide;
        _hold_mode holdMode;
        double_type total;
        double_type available;
        double_type locked;
        double_type avgOpenPrice;
        int leverage;
        double_type achievedProfits;
        double_type uPL;
        double_type uPLRate;
        double_type liqPrice;
        double_type keepMarginRate;
        double_type fixedMarginRate;
        double_type marginRate;
        std::size_t cTime;
        std::size_t uTime;

        static position_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const position_t &f);
    };

    struct spot_order_t {
        std::string instId;
        std::string orderId;
        std::string clientOrderId;
        double_type price;
        double_type size;
        double_type notional;
        _force force;
        _side side;
        double_type fillPrice;
        std::string tradeId;
        double_type fillSize;
        std::size_t fillTime;
        double_type fillFee;
        std::string fillFeeCcy;
        std::string execType;   // Look at turning this into a bool where Maker is 0 and Taker is 1
        double_type accFillSize;
        double_type avgPrice;
        _status status;
        std::size_t cTime;
        std::size_t uTime;
        std::map<std::string, double_type> feeList;

        static spot_order_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const spot_order_t &f);
    };

    struct futures_order_t {
        std::string instId;
        std::string orderId;
        std::string clientOrderId;
        double_type price;
        double_type size;
        double_type notional;
        _order_type orderType;
        _force force;
        _hold_side posSide;
        _margin_mode tdMode;
        std::string tgtCcy;
        double_type fillPrice;
        std::string tradeId;
        double_type fillSize;
        std::size_t fillTime;
        double_type fillFee;
        std::string fillFeeCcy;
        std::string execType;
        double_type accFillSize;
        double_type fillNotional;
        _status status;
        int leverage;
        std::map<std::string, double_type> feeList;
        double_type pnl;
        std::size_t uTime;
        std::size_t cTime;

        static futures_order_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const futures_order_t &f);
    };

    struct plan_order_t {
        std::string instId;
        std::string orderId;
        std::string clientOrderId;
        double_type triggerPrice;
        _ws_plan_type planType;
        double_type orderPrice;
        double_type size;
        double_type actualSize;
        _order_type orderType;
        _side side;
        _hold_side posSide;
        std::string tgtCcy;
        _plan_status status;
        std::size_t uTime;
        std::size_t cTime;

        static plan_order_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const plan_order_t &f);
    };

//------------------------------------------------------------------------------

} // ns ws

} // ns bg_api

#endif // __bg_api__types_hpp
