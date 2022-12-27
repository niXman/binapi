
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
    using coin_list_cb = std::function<bool(const char *fl, int ec, std::string errmsg, coin_list_t res)>;
    result<coin_list_t> getCoinList(coin_list_cb cb = {});

    // https://bitgetlimited.github.io/apidoc/en/spot/#get-symbols
    using symbols_cb = std::function<bool(const char *fl, int ec, std::string errmsg, symbols_t res)>;
    result<symbols_t> getSymbols(symbols_cb cb = {});

    // https://bitgetlimited.github.io/apidoc/en/spot/#get-single-symbol
    using symbol_cb = std::function<bool(const char *fl, int ec, std::string errmsg, symbol_t res)>;
    result<symbol_t> getSymbol(const std::string symbol, symbol_cb cb = {});

    // https://bitgetlimited.github.io/apidoc/en/spot/#get-single-ticker
    using ticker_cb = std::function<bool(const char *fl, int ec, std::string errmsg, spot_ticker_t res)>;
    result<spot_ticker_t> getTicker(const std::string symbol, ticker_cb cb = {});

    // https://bitgetlimited.github.io/apidoc/en/spot/#get-all-tickers
    using tickers_cb = std::function<bool(const char *fl, int ec, std::string errmsg, spot_ticker_t res)>;
    result<std::vector<spot_ticker_t>> getTickers(tickers_cb cb = {});

    // https://bitgetlimited.github.io/apidoc/en/spot/#get-market-trades
    
    // https://bitgetlimited.github.io/apidoc/en/spot/#get-candle-data

    // https://bitgetlimited.github.io/apidoc/en/spot/#get-depth

    // https://bitgetlimited.github.io/apidoc/en/spot/#transfer

    // https://bitgetlimited.github.io/apidoc/en/spot/#sub-transfer

    // https://bitgetlimited.github.io/apidoc/en/spot/#get-coin-address

    // https://bitgetlimited.github.io/apidoc/en/spot/#withdraw

    // https://bitgetlimited.github.io/apidoc/en/spot/#inner-withdraw

    // https://bitgetlimited.github.io/apidoc/en/spot/#get-withdraw-list

    // https://bitgetlimited.github.io/apidoc/en/spot/#get-deposit-list

    // https://bitgetlimited.github.io/apidoc/en/spot/#get-apikey-info

    // https://bitgetlimited.github.io/apidoc/en/spot/#get-account

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


private:
    struct impl;
    std::unique_ptr<impl> pimpl;
};

/*************************************************************************************************/

} // ns rest
} // ns bg_api

#endif // __bg_api__api_hpp
