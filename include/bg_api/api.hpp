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

#ifndef __bg_api__api_hpp
#define __bg_api__api_hpp

#include "types.hpp"
#include "enums.hpp"

#include <memory>
#include <functional>

namespace boost {
namespace asio {

class io_context;

} // ns asio
} // ns boost

namespace bg_api {
namespace rest {

/*************************************************************************************************/

struct api {
    template<typename T>
    struct result {
        result()
            :ec{0}
        {}

        int ec;
        std::string errmsg;
        std::string reply;
        T v;

        // returns FALSE when error
        explicit operator bool() const { return errmsg.empty(); }
    };

    api(
        boost::asio::io_context &ioctx,
        std::string host,
        std::string port,
        std::string pk,
        std::string passphrase,
        std::string sk,
        std::size_t timeout,
        std::string locale = "en-US",
        std::string client_api_string = "bg_api-0.0.1"
    );
    virtual ~api();

    api(const api &) = delete;
    api(api &&) = default;

    // https://bitgetlimited.github.io/apidoc/en/spot/#get-server-time
    using server_time_cb = std::function<bool(const char *fl, int ec, std::string errmsg, server_time_t res)>;
    result<server_time_t> getServerTime(server_time_cb cb = {});
    
    // https://bitgetlimited.github.io/apidoc/en/spot/#get-coin-list
    using currencies_cb = std::function<bool(const char *fl, int ec, std::string errmsg, currencies_t res)>;
    result<currencies_t> getCurrencies(currencies_cb cb = {});

    // https://bitgetlimited.github.io/apidoc/en/spot/#get-single-symbol
    using product_cb = std::function<bool(const char *fl, int ec, std::string errmsg, product_t res)>;
    result<product_t> getProduct(const char* symbol, product_cb cb = {});

    // https://bitgetlimited.github.io/apidoc/en/spot/#get-symbols
    using products_cb = std::function<bool(const char *fl, int ec, std::string errmsg, products_t res)>;
    result<products_t> getProducts(products_cb cb = {});
    
    // https://bitgetlimited.github.io/apidoc/en/spot/#get-single-ticker
    using spot_ticker_cb = std::function<bool(const char *fl, int ec, std::string errmsg, spot_ticker_t res)>;
    result<spot_ticker_t> getSpotTicker(const char* symbol, spot_ticker_cb cb = {});
    
    // https://bitgetlimited.github.io/apidoc/en/spot/#get-all-tickers
    using spot_tickers_cb = std::function<bool(const char *fl, int ec, std::string errmsg, spot_tickers_t res)>;
    result<spot_tickers_t> getSpotTickers(spot_tickers_cb cb = {});
    
    // https://bitgetlimited.github.io/apidoc/en/spot/#get-market-trades
    using spot_fills_cb = std::function<bool(const char *fl, int ec, std::string errmsg, spot_fills_t res)>;
    result<spot_fills_t> getSpotFills(const char* symbol, spot_fills_cb cb = {}, int limit = 100);
    
    // https://bitgetlimited.github.io/apidoc/en/spot/#get-candle-data
    using spot_candles_cb = std::function<bool(const char *fl, int ec, std::string errmsg, spot_candles_t res)>;
    result<spot_candles_t> getSpotCandles(const char* symbol, _candle_gran period, spot_candles_cb cb = {}, time_t after = 0, time_t before = 0, int limit = 100);
    
    // https://bitgetlimited.github.io/apidoc/en/spot/#get-depth
    using spot_depth_cb = std::function<bool(const char *fl, int ec, std::string errmsg, spot_depth_t res)>;
    result<spot_depth_t> getSpotDepth(const char* symbol, spot_depth_cb cb = {}, int type = 0, int limit = 150);
    
    // https://bitgetlimited.github.io/apidoc/en/spot/#vip-fee-rate
    using spot_vip_rates_cb = std::function<bool(const char *fl, int ec, std::string errmsg, spot_vip_rates_t res)>;
    result<spot_vip_rates_t> getSpotVipRates(spot_vip_rates_cb cb = {});

    // https://bitgetlimited.github.io/apidoc/en/spot/#transfer
    using transfer_cb = std::function<bool(const char *fl, int ec, std::string errmsg, transfer_res_t res)>;
    result<transfer_res_t> transfer(transfer_req_t req, transfer_cb cb = {});
    
    // https://bitgetlimited.github.io/apidoc/en/spot/#get-coin-address
    using deposit_address_cb = std::function<bool(const char *fl, int ec, std::string errmsg, deposit_address_t res)>;
    result<deposit_address_t> getDepositAddress(const char* coin, const char* chain, deposit_address_cb cb = {});
    
    // https://bitgetlimited.github.io/apidoc/en/spot/#withdraw
    using withdraw_cb = std::function<bool(const char *fl, int ec, std::string errmsg, withdraw_res_t res)>;
    result<withdraw_res_t> withdraw(withdraw_req_t req, withdraw_cb cb = {});
    
    // https://bitgetlimited.github.io/apidoc/en/spot/#inner-withdraw
    using inner_withdraw_cb = std::function<bool(const char *fl, int ec, std::string errmsg, withdraw_res_t res)>;
    result<withdraw_res_t> innerWithdraw(inner_withdraw_req_t req, inner_withdraw_cb cb = {});
    
    // https://bitgetlimited.github.io/apidoc/en/spot/#get-withdraw-list
    using withdraw_list_cb = std::function<bool(const char *fl, int ec, std::string errmsg, tx_list_t res)>;
    result<tx_list_t> getWithdrawList(const char* coin, time_t startTime, time_t endTime, withdraw_list_cb cb = {}, int pageNo = 1, int pageSize = 20);
    
    // https://bitgetlimited.github.io/apidoc/en/spot/#get-deposit-list
    using deposit_list_cb = std::function<bool(const char *fl, int ec, std::string errmsg, tx_list_t res)>;
    result<tx_list_t> getDepositList(const char* coin, time_t startTime, time_t endTime, deposit_list_cb cb = {}, int pageNo = 1, int pageSize = 20);
    
    // https://bitgetlimited.github.io/apidoc/en/spot/#get-apikey-info
    using apikey_cb = std::function<bool(const char *fl, int ec, std::string errmsg, apikey_t res)>;
    result<apikey_t> getApikeyInfo(apikey_cb cb = {});
    
    // https://bitgetlimited.github.io/apidoc/en/spot/#get-account-assets
    using assets_cb = std::function<bool(const char *fl, int ec, std::string errmsg, assets_t res)>;
    result<assets_t> getAssets(assets_cb cb = {}, const char* coin = "");
    
    // https://bitgetlimited.github.io/apidoc/en/spot/#get-bills
    
    // https://bitgetlimited.github.io/apidoc/en/spot/#get-transfer-list
    
    // https://bitgetlimited.github.io/apidoc/en/spot/#place-order
    
    // https://bitgetlimited.github.io/apidoc/en/spot/#batch-order

    // https://bitgetlimited.github.io/apidoc/en/spot/#cancel-order
    
    // https://bitgetlimited.github.io/apidoc/en/spot/#cancel-order-in-batch-single-instruments
    
    // https://bitgetlimited.github.io/apidoc/en/spot/#get-order-details
    
    // https://bitgetlimited.github.io/apidoc/en/spot/#get-order-list
    
    // https://bitgetlimited.github.io/apidoc/en/spot/#get-order-history
    
    // https://bitgetlimited.github.io/apidoc/en/spot/#get-transaction-details
    
    // https://bitgetlimited.github.io/apidoc/en/spot/#place-plan-order
    
    // https://bitgetlimited.github.io/apidoc/en/spot/#modify-plan-order
    
    // https://bitgetlimited.github.io/apidoc/en/spot/#cancel-plan-order
    
    // https://bitgetlimited.github.io/apidoc/en/spot/#get-current-plan-orders
    
    // https://bitgetlimited.github.io/apidoc/en/spot/#get-history-plan-orders
    

/*************************************************************************************************/

    // https://bitgetlimited.github.io/apidoc/en/mix/#get-all-symbols
    
    // https://bitgetlimited.github.io/apidoc/en/mix/#get-depth
    
    // https://bitgetlimited.github.io/apidoc/en/mix/#get-single-symbol-ticker
    
    // https://bitgetlimited.github.io/apidoc/en/mix/#get-all-symbol-ticker
    
    // https://bitgetlimited.github.io/apidoc/en/mix/#get-fills
    
    // https://bitgetlimited.github.io/apidoc/en/mix/#get-candle-data
    
    // https://bitgetlimited.github.io/apidoc/en/mix/#get-symbol-index-price
    
    // https://bitgetlimited.github.io/apidoc/en/mix/#get-symbol-next-funding-time
    
    // https://bitgetlimited.github.io/apidoc/en/mix/#get-history-funding-rate
    
    // https://bitgetlimited.github.io/apidoc/en/mix/#get-current-funding-rate
    
    // https://bitgetlimited.github.io/apidoc/en/mix/#get-open-interest

    // https://bitgetlimited.github.io/apidoc/en/mix/#get-symbol-mark-price
    
    // https://bitgetlimited.github.io/apidoc/en/mix/#get-symbol-leverage
    
    // https://bitgetlimited.github.io/apidoc/en/mix/#get-single-account
    
    // https://bitgetlimited.github.io/apidoc/en/mix/#get-account-list
    
    // https://bitgetlimited.github.io/apidoc/en/mix/#get-open-count
    
    // https://bitgetlimited.github.io/apidoc/en/mix/#change-leverage
    
    // https://bitgetlimited.github.io/apidoc/en/mix/#change-margin
    
    // https://bitgetlimited.github.io/apidoc/en/mix/#change-margin-mode
    
    // https://bitgetlimited.github.io/apidoc/en/mix/#change-hold-mode
    
    // https://bitgetlimited.github.io/apidoc/en/mix/#get-symbol-position
    
    // https://bitgetlimited.github.io/apidoc/en/mix/#get-all-position
    
    // https://bitgetlimited.github.io/apidoc/en/mix/#get-account-bill
    
    // https://bitgetlimited.github.io/apidoc/en/mix/#get-business-account-bill
    
    // https://bitgetlimited.github.io/apidoc/en/mix/#place-order

    // https://bitgetlimited.github.io/apidoc/en/mix/#reversal
    
    // https://bitgetlimited.github.io/apidoc/en/mix/#batch-order

    // https://bitgetlimited.github.io/apidoc/en/mix/#cancel-order

    // https://bitgetlimited.github.io/apidoc/en/mix/#batch-cancel-order

    // https://bitgetlimited.github.io/apidoc/en/mix/#cancel-all-order

    // https://bitgetlimited.github.io/apidoc/en/mix/#get-open-order

    // https://bitgetlimited.github.io/apidoc/en/mix/#get-all-open-order

    // https://bitgetlimited.github.io/apidoc/en/mix/#get-history-orders

    // https://bitgetlimited.github.io/apidoc/en/mix/#get-producttype-history-orders

    // https://bitgetlimited.github.io/apidoc/en/mix/#get-order-details

    // https://bitgetlimited.github.io/apidoc/en/mix/#get-order-fill-detail

    // https://bitgetlimited.github.io/apidoc/en/mix/#get-producttype-order-fill-detail

    // https://bitgetlimited.github.io/apidoc/en/mix/#place-plan-order

    // https://bitgetlimited.github.io/apidoc/en/mix/#modify-plan-order

    // https://bitgetlimited.github.io/apidoc/en/mix/#modify-plan-order-tpsl

    // https://bitgetlimited.github.io/apidoc/en/mix/#place-stop-order

    // https://bitgetlimited.github.io/apidoc/en/mix/#place-trailing-stop-order

    // https://bitgetlimited.github.io/apidoc/en/mix/#place-position-tpsl

    // https://bitgetlimited.github.io/apidoc/en/mix/#modify-stop-order

    // https://bitgetlimited.github.io/apidoc/en/mix/#cancel-plan-order-tpsl

    // https://bitgetlimited.github.io/apidoc/en/mix/#cancel-all-trigger-order-tpsl

    // https://bitgetlimited.github.io/apidoc/en/mix/#get-plan-order-tpsl-list

    // https://bitgetlimited.github.io/apidoc/en/mix/#get-history-plan-orders-tpsl

    // https://bitgetlimited.github.io/apidoc/en/mix/#get-trader-open-order

    // https://bitgetlimited.github.io/apidoc/en/mix/#get-follower-open-orders

    // https://bitgetlimited.github.io/apidoc/en/mix/#get-follower-history-orders

    // https://bitgetlimited.github.io/apidoc/en/mix/#trader-close-position

    // https://bitgetlimited.github.io/apidoc/en/mix/#trader-modify-tpsl

    // https://bitgetlimited.github.io/apidoc/en/mix/#get-traders-history-orders

    // https://bitgetlimited.github.io/apidoc/en/mix/#get-trader-profit-summary

    // https://bitgetlimited.github.io/apidoc/en/mix/#get-trader-history-profit-summary-according-to-settlement-currency

    // https://bitgetlimited.github.io/apidoc/en/mix/#get-trader-history-profit-summary-according-to-settlement-currency-and-date

    // https://bitgetlimited.github.io/apidoc/en/mix/#get-trader-history-profit-detail

    // https://bitgetlimited.github.io/apidoc/en/mix/#get-trader-profits-details

    // https://bitgetlimited.github.io/apidoc/en/mix/#get-copytrade-symbols

    // https://bitgetlimited.github.io/apidoc/en/mix/#trader-change-copytrade-symbol

private:
    struct impl;
    std::unique_ptr<impl> pimpl;
};

/*************************************************************************************************/

} // ns rest
} // ns bg_api

#endif // __bg_api__api_hpp
