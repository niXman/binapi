
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
    result<symbol_t> getSymbol(const std::string &symbol, symbol_cb cb = {}) { return getSymbol(symbol.c_str(), cb); }
    result<symbol_t> getSymbol(const char* symbol, symbol_cb cb = {});

    // https://bitgetlimited.github.io/apidoc/en/spot/#get-single-ticker
    using spot_ticker_cb = std::function<bool(const char *fl, int ec, std::string errmsg, spot_ticker_t res)>;
    result<spot_ticker_t> getSpotTicker(const std::string &symbol, spot_ticker_cb cb = {}) { return getSpotTicker(symbol.c_str(), cb); }
    result<spot_ticker_t> getSpotTicker(const char* symbol, spot_ticker_cb cb = {});

    // https://bitgetlimited.github.io/apidoc/en/spot/#get-all-tickers
    using spot_tickers_cb = std::function<bool(const char *fl, int ec, std::string errmsg, spot_tickers_t res)>;
    result<spot_tickers_t> getSpotTickers(spot_tickers_cb cb = {});

    // https://bitgetlimited.github.io/apidoc/en/spot/#get-market-trades
    using spot_trades_cb = std::function<bool(const char *fl, int ec, std::string errmsg, trades_t res)>;
    result<trades_t> getSpotTrades(const std::string &symbol, spot_trades_cb cb = {}, uint16_t limit = 100) { return getSpotTrades(symbol.c_str(), cb, limit); }
    result<trades_t> getSpotTrades(const char* symbol, spot_trades_cb cb = {}, uint16_t limit = 100);
    
    // https://bitgetlimited.github.io/apidoc/en/spot/#get-candle-data
    using candles_cb = std::function<bool(const char *fl, int ec, std::string errmsg, candles_t res)>;
    result<candles_t> getSpotCandles(const std::string &symbol, const std::string &period, candles_cb cb = {}, std::size_t startTime = 0, std::size_t endTime = 0, uint16_t limit = 100) { return getSpotCandles(symbol.c_str(), candle_gran_from_string(period), cb, startTime, endTime, limit); }
    result<candles_t> getSpotCandles(const std::string &symbol, const char* period, candles_cb cb = {}, std::size_t startTime = 0, std::size_t endTime = 0, uint16_t limit = 100) { return getSpotCandles(symbol.c_str(), candle_gran_from_string(period), cb, startTime, endTime, limit); }
    result<candles_t> getSpotCandles(const std::string &symbol, std::size_t period, candles_cb cb = {}, std::size_t startTime = 0, std::size_t endTime = 0, uint16_t limit = 100) { return getSpotCandles(symbol.c_str(), static_cast<_candle_gran>(period), cb, startTime, endTime, limit); }
    result<candles_t> getSpotCandles(const std::string &symbol, _candle_gran period, candles_cb cb = {}, std::size_t startTime = 0, std::size_t endTime = 0, uint16_t limit = 100) { return getSpotCandles(symbol.c_str(), period, cb, startTime, endTime, limit); }
    result<candles_t> getSpotCandles(const char* symbol, const std::string &period, candles_cb cb = {}, std::size_t startTime = 0, std::size_t endTime = 0, uint16_t limit = 100) { return getSpotCandles(symbol, candle_gran_from_string(period), cb, startTime, endTime, limit); }
    result<candles_t> getSpotCandles(const char* symbol, const char* period, candles_cb cb = {}, std::size_t startTime = 0, std::size_t endTime = 0, uint16_t limit = 100) { return getSpotCandles(symbol, candle_gran_from_string(period), cb, startTime, endTime, limit); }
    result<candles_t> getSpotCandles(const char* symbol, std::size_t period, candles_cb cb = {}, std::size_t startTime = 0, std::size_t endTime = 0, uint16_t limit = 100) { return getSpotCandles(symbol, static_cast<_candle_gran>(period), cb, startTime, endTime, limit); }
    result<candles_t> getSpotCandles(const char* symbol, _candle_gran period, candles_cb cb = {}, std::size_t startTime = 0, std::size_t endTime = 0, uint16_t limit = 100);

    // https://bitgetlimited.github.io/apidoc/en/spot/#get-depth
    // Note that this doesn't support different step types due to the json returned having different value types
    // This may or may not be compensated in a future version
    using depth_cb = std::function<bool(const char *fl, int ec, std::string errmsg, depth_t res)>;
    result<depth_t> getSpotDepth(const std::string &symbol, depth_cb cb = {}, uint16_t limit = 100) { return getSpotDepth(symbol.c_str(), cb, limit); }
    result<depth_t> getSpotDepth(const char* symbol, depth_cb cb = {}, uint16_t limit = 100);

    // https://bitgetlimited.github.io/apidoc/en/spot/#transfer
    using transfer_cb = std::function<bool(const char *fl, int ec, std::string errmsg, transfer_res_t res)>;
    result<transfer_res_t> transfer(_from_to_type from, _from_to_type to, double_type amount, const std::string &coin, transfer_cb cb = {}, std::string clientOid = "") { return transfer(from, to, amount, coin.c_str(), cb, clientOid.c_str()); }
    result<transfer_res_t> transfer(_from_to_type from, _from_to_type to, double_type amount, const std::string &coin, transfer_cb cb = {}, const char* clientOid = "") { return transfer(from, to, amount, coin.c_str(), cb, clientOid); }
    result<transfer_res_t> transfer(_from_to_type from, _from_to_type to, double_type amount, const char* coin, transfer_cb cb = {}, std::string clientOid = "") { return transfer(from, to, amount, coin, cb, clientOid.c_str()); }
    result<transfer_res_t> transfer(_from_to_type from, _from_to_type to, double_type amount, const char* coin, transfer_cb cb = {}, const char* clientOid = "");

    // https://bitgetlimited.github.io/apidoc/en/spot/#get-coin-address
    using address_cb = std::function<bool(const char *fl, int ec, std::string errmsg, address_t res)>;
    result<address_t> getAddress(const std::string &coin, address_cb cb = {}, const std::string& chain = "") { return getAddress(coin.c_str(), cb, chain.c_str()); }
    result<address_t> getAddress(const std::string &coin, address_cb cb = {}, const char* chain = "") { return getAddress(coin.c_str(), cb, chain); }
    result<address_t> getAddress(const char* coin, address_cb cb = {}, const std::string& chain = "") { return getAddress(coin, cb, chain.c_str()); }
    result<address_t> getAddress(const char* coin, address_cb cb = {}, const char* chain = "");

    // https://bitgetlimited.github.io/apidoc/en/spot/#withdraw
    using withdraw_cb = std::function<bool(const char *fl, int ec, std::string errmsg, withdraw_res_t res)>;
    result<withdraw_res_t> withdraw(const std::string &coin, const std::string &address, const std::string &chain, double_type amount, withdraw_cb cb = {}, const std::string tag = "", const std::string remark = "", const std::string clientOid = "") { return withdraw(coin.c_str(), address.c_str(), chain.c_str(), amount, cb, tag.c_str(), remark.c_str(), clientOid.c_str()); }
    result<withdraw_res_t> withdraw(const std::string &coin, const std::string &address, const std::string &chain, double_type amount, withdraw_cb cb = {}, const std::string tag = "", const std::string remark = "", const char* clientOid = "") { return withdraw(coin.c_str(), address.c_str(), chain.c_str(), amount, cb, tag.c_str(), remark.c_str(), clientOid); }
    result<withdraw_res_t> withdraw(const std::string &coin, const std::string &address, const std::string &chain, double_type amount, withdraw_cb cb = {}, const std::string tag = "", const char* remark = "", const std::string clientOid = "") { return withdraw(coin.c_str(), address.c_str(), chain.c_str(), amount, cb, tag.c_str(), remark, clientOid.c_str()); }
    result<withdraw_res_t> withdraw(const std::string &coin, const std::string &address, const std::string &chain, double_type amount, withdraw_cb cb = {}, const std::string tag = "", const char* remark = "", const char* clientOid = "") { return withdraw(coin.c_str(), address.c_str(), chain.c_str(), amount, cb, tag.c_str(), remark, clientOid); }
    result<withdraw_res_t> withdraw(const std::string &coin, const std::string &address, const std::string &chain, double_type amount, withdraw_cb cb = {}, const char* tag = "", const std::string remark = "", const std::string clientOid = "") { return withdraw(coin.c_str(), address.c_str(), chain.c_str(), amount, cb, tag, remark.c_str(), clientOid.c_str()); }
    result<withdraw_res_t> withdraw(const std::string &coin, const std::string &address, const std::string &chain, double_type amount, withdraw_cb cb = {}, const char* tag = "", const std::string remark = "", const char* clientOid = "") { return withdraw(coin.c_str(), address.c_str(), chain.c_str(), amount, cb, tag, remark.c_str(), clientOid); }
    result<withdraw_res_t> withdraw(const std::string &coin, const std::string &address, const std::string &chain, double_type amount, withdraw_cb cb = {}, const char* tag = "", const char* remark = "", const std::string clientOid = "") { return withdraw(coin.c_str(), address.c_str(), chain.c_str(), amount, cb, tag, remark, clientOid.c_str()); }
    result<withdraw_res_t> withdraw(const std::string &coin, const std::string &address, const std::string &chain, double_type amount, withdraw_cb cb = {}, const char* tag = "", const char* remark = "", const char* clientOid = "") { return withdraw(coin.c_str(), address.c_str(), chain.c_str(), amount, cb, tag, remark, clientOid); }
    result<withdraw_res_t> withdraw(const std::string &coin, const std::string &address, const char* chain, double_type amount, withdraw_cb cb = {}, const std::string tag = "", const std::string remark = "", const std::string clientOid = "") { return withdraw(coin.c_str(), address.c_str(), chain, amount, cb, tag.c_str(), remark.c_str(), clientOid.c_str()); }
    result<withdraw_res_t> withdraw(const std::string &coin, const std::string &address, const char* chain, double_type amount, withdraw_cb cb = {}, const std::string tag = "", const std::string remark = "", const char* clientOid = "") { return withdraw(coin.c_str(), address.c_str(), chain, amount, cb, tag.c_str(), remark.c_str(), clientOid); }
    result<withdraw_res_t> withdraw(const std::string &coin, const std::string &address, const char* chain, double_type amount, withdraw_cb cb = {}, const std::string tag = "", const char* remark = "", const std::string clientOid = "") { return withdraw(coin.c_str(), address.c_str(), chain, amount, cb, tag.c_str(), remark, clientOid.c_str()); }
    result<withdraw_res_t> withdraw(const std::string &coin, const std::string &address, const char* chain, double_type amount, withdraw_cb cb = {}, const std::string tag = "", const char* remark = "", const char* clientOid = "") { return withdraw(coin.c_str(), address.c_str(), chain, amount, cb, tag.c_str(), remark, clientOid); }
    result<withdraw_res_t> withdraw(const std::string &coin, const std::string &address, const char* chain, double_type amount, withdraw_cb cb = {}, const char* tag = "", const std::string remark = "", const std::string clientOid = "") { return withdraw(coin.c_str(), address.c_str(), chain, amount, cb, tag, remark.c_str(), clientOid.c_str()); }
    result<withdraw_res_t> withdraw(const std::string &coin, const std::string &address, const char* chain, double_type amount, withdraw_cb cb = {}, const char* tag = "", const std::string remark = "", const char* clientOid = "") { return withdraw(coin.c_str(), address.c_str(), chain, amount, cb, tag, remark.c_str(), clientOid); }
    result<withdraw_res_t> withdraw(const std::string &coin, const std::string &address, const char* chain, double_type amount, withdraw_cb cb = {}, const char* tag = "", const char* remark = "", const std::string clientOid = "") { return withdraw(coin.c_str(), address.c_str(), chain, amount, cb, tag, remark, clientOid.c_str()); }
    result<withdraw_res_t> withdraw(const std::string &coin, const std::string &address, const char* chain, double_type amount, withdraw_cb cb = {}, const char* tag = "", const char* remark = "", const char* clientOid = "") { return withdraw(coin.c_str(), address.c_str(), chain, amount, cb, tag, remark, clientOid); }
    result<withdraw_res_t> withdraw(const std::string &coin, const char* address, const std::string &chain, double_type amount, withdraw_cb cb = {}, const std::string tag = "", const std::string remark = "", const std::string clientOid = "") { return withdraw(coin.c_str(), address, chain.c_str(), amount, cb, tag.c_str(), remark.c_str(), clientOid.c_str()); }
    result<withdraw_res_t> withdraw(const std::string &coin, const char* address, const std::string &chain, double_type amount, withdraw_cb cb = {}, const std::string tag = "", const std::string remark = "", const char* clientOid = "") { return withdraw(coin.c_str(), address, chain.c_str(), amount, cb, tag.c_str(), remark.c_str(), clientOid); }
    result<withdraw_res_t> withdraw(const std::string &coin, const char* address, const std::string &chain, double_type amount, withdraw_cb cb = {}, const std::string tag = "", const char* remark = "", const std::string clientOid = "") { return withdraw(coin.c_str(), address, chain.c_str(), amount, cb, tag.c_str(), remark, clientOid.c_str()); }
    result<withdraw_res_t> withdraw(const std::string &coin, const char* address, const std::string &chain, double_type amount, withdraw_cb cb = {}, const std::string tag = "", const char* remark = "", const char* clientOid = "") { return withdraw(coin.c_str(), address, chain.c_str(), amount, cb, tag.c_str(), remark, clientOid); }
    result<withdraw_res_t> withdraw(const std::string &coin, const char* address, const std::string &chain, double_type amount, withdraw_cb cb = {}, const char* tag = "", const std::string remark = "", const std::string clientOid = "") { return withdraw(coin.c_str(), address, chain.c_str(), amount, cb, tag, remark.c_str(), clientOid.c_str()); }
    result<withdraw_res_t> withdraw(const std::string &coin, const char* address, const std::string &chain, double_type amount, withdraw_cb cb = {}, const char* tag = "", const std::string remark = "", const char* clientOid = "") { return withdraw(coin.c_str(), address, chain.c_str(), amount, cb, tag, remark.c_str(), clientOid); }
    result<withdraw_res_t> withdraw(const std::string &coin, const char* address, const std::string &chain, double_type amount, withdraw_cb cb = {}, const char* tag = "", const char* remark = "", const std::string clientOid = "") { return withdraw(coin.c_str(), address, chain.c_str(), amount, cb, tag, remark, clientOid.c_str()); }
    result<withdraw_res_t> withdraw(const std::string &coin, const char* address, const std::string &chain, double_type amount, withdraw_cb cb = {}, const char* tag = "", const char* remark = "", const char* clientOid = "") { return withdraw(coin.c_str(), address, chain.c_str(), amount, cb, tag, remark, clientOid); }
    result<withdraw_res_t> withdraw(const std::string &coin, const char* address, const char* chain, double_type amount, withdraw_cb cb = {}, const std::string tag = "", const std::string remark = "", const std::string clientOid = "") { return withdraw(coin.c_str(), address, chain, amount, cb, tag.c_str(), remark.c_str(), clientOid.c_str()); }
    result<withdraw_res_t> withdraw(const std::string &coin, const char* address, const char* chain, double_type amount, withdraw_cb cb = {}, const std::string tag = "", const std::string remark = "", const char* clientOid = "") { return withdraw(coin.c_str(), address, chain, amount, cb, tag.c_str(), remark.c_str(), clientOid); }
    result<withdraw_res_t> withdraw(const std::string &coin, const char* address, const char* chain, double_type amount, withdraw_cb cb = {}, const std::string tag = "", const char* remark = "", const std::string clientOid = "") { return withdraw(coin.c_str(), address, chain, amount, cb, tag.c_str(), remark, clientOid.c_str()); }
    result<withdraw_res_t> withdraw(const std::string &coin, const char* address, const char* chain, double_type amount, withdraw_cb cb = {}, const std::string tag = "", const char* remark = "", const char* clientOid = "") { return withdraw(coin.c_str(), address, chain, amount, cb, tag.c_str(), remark, clientOid); }
    result<withdraw_res_t> withdraw(const std::string &coin, const char* address, const char* chain, double_type amount, withdraw_cb cb = {}, const char* tag = "", const std::string remark = "", const std::string clientOid = "") { return withdraw(coin.c_str(), address, chain, amount, cb, tag, remark.c_str(), clientOid.c_str()); }
    result<withdraw_res_t> withdraw(const std::string &coin, const char* address, const char* chain, double_type amount, withdraw_cb cb = {}, const char* tag = "", const std::string remark = "", const char* clientOid = "") { return withdraw(coin.c_str(), address, chain, amount, cb, tag, remark.c_str(), clientOid); }
    result<withdraw_res_t> withdraw(const std::string &coin, const char* address, const char* chain, double_type amount, withdraw_cb cb = {}, const char* tag = "", const char* remark = "", const std::string clientOid = "") { return withdraw(coin.c_str(), address, chain, amount, cb, tag, remark, clientOid.c_str()); }
    result<withdraw_res_t> withdraw(const std::string &coin, const char* address, const char* chain, double_type amount, withdraw_cb cb = {}, const char* tag = "", const char* remark = "", const char* clientOid = "") { return withdraw(coin.c_str(), address, chain, amount, cb, tag, remark, clientOid); }
    result<withdraw_res_t> withdraw(const char* coin, const std::string &address, const std::string &chain, double_type amount, withdraw_cb cb = {}, const std::string tag = "", const std::string remark = "", const std::string clientOid = "") { return withdraw(coin, address.c_str(), chain.c_str(), amount, cb, tag.c_str(), remark.c_str(), clientOid.c_str()); }
    result<withdraw_res_t> withdraw(const char* coin, const std::string &address, const std::string &chain, double_type amount, withdraw_cb cb = {}, const std::string tag = "", const std::string remark = "", const char* clientOid = "") { return withdraw(coin, address.c_str(), chain.c_str(), amount, cb, tag.c_str(), remark.c_str(), clientOid); }
    result<withdraw_res_t> withdraw(const char* coin, const std::string &address, const std::string &chain, double_type amount, withdraw_cb cb = {}, const std::string tag = "", const char* remark = "", const std::string clientOid = "") { return withdraw(coin, address.c_str(), chain.c_str(), amount, cb, tag.c_str(), remark, clientOid.c_str()); }
    result<withdraw_res_t> withdraw(const char* coin, const std::string &address, const std::string &chain, double_type amount, withdraw_cb cb = {}, const std::string tag = "", const char* remark = "", const char* clientOid = "") { return withdraw(coin, address.c_str(), chain.c_str(), amount, cb, tag.c_str(), remark, clientOid); }
    result<withdraw_res_t> withdraw(const char* coin, const std::string &address, const std::string &chain, double_type amount, withdraw_cb cb = {}, const char* tag = "", const std::string remark = "", const std::string clientOid = "") { return withdraw(coin, address.c_str(), chain.c_str(), amount, cb, tag, remark.c_str(), clientOid.c_str()); }
    result<withdraw_res_t> withdraw(const char* coin, const std::string &address, const std::string &chain, double_type amount, withdraw_cb cb = {}, const char* tag = "", const std::string remark = "", const char* clientOid = "") { return withdraw(coin, address.c_str(), chain.c_str(), amount, cb, tag, remark.c_str(), clientOid); }
    result<withdraw_res_t> withdraw(const char* coin, const std::string &address, const std::string &chain, double_type amount, withdraw_cb cb = {}, const char* tag = "", const char* remark = "", const std::string clientOid = "") { return withdraw(coin, address.c_str(), chain.c_str(), amount, cb, tag, remark, clientOid.c_str()); }
    result<withdraw_res_t> withdraw(const char* coin, const std::string &address, const std::string &chain, double_type amount, withdraw_cb cb = {}, const char* tag = "", const char* remark = "", const char* clientOid = "") { return withdraw(coin, address.c_str(), chain.c_str(), amount, cb, tag, remark, clientOid); }
    result<withdraw_res_t> withdraw(const char* coin, const std::string &address, const char* chain, double_type amount, withdraw_cb cb = {}, const std::string tag = "", const std::string remark = "", const std::string clientOid = "") { return withdraw(coin, address.c_str(), chain, amount, cb, tag.c_str(), remark.c_str(), clientOid.c_str()); }
    result<withdraw_res_t> withdraw(const char* coin, const std::string &address, const char* chain, double_type amount, withdraw_cb cb = {}, const std::string tag = "", const std::string remark = "", const char* clientOid = "") { return withdraw(coin, address.c_str(), chain, amount, cb, tag.c_str(), remark.c_str(), clientOid); }
    result<withdraw_res_t> withdraw(const char* coin, const std::string &address, const char* chain, double_type amount, withdraw_cb cb = {}, const std::string tag = "", const char* remark = "", const std::string clientOid = "") { return withdraw(coin, address.c_str(), chain, amount, cb, tag.c_str(), remark, clientOid.c_str()); }
    result<withdraw_res_t> withdraw(const char* coin, const std::string &address, const char* chain, double_type amount, withdraw_cb cb = {}, const std::string tag = "", const char* remark = "", const char* clientOid = "") { return withdraw(coin, address.c_str(), chain, amount, cb, tag.c_str(), remark, clientOid); }
    result<withdraw_res_t> withdraw(const char* coin, const std::string &address, const char* chain, double_type amount, withdraw_cb cb = {}, const char* tag = "", const std::string remark = "", const std::string clientOid = "") { return withdraw(coin, address.c_str(), chain, amount, cb, tag, remark.c_str(), clientOid.c_str()); }
    result<withdraw_res_t> withdraw(const char* coin, const std::string &address, const char* chain, double_type amount, withdraw_cb cb = {}, const char* tag = "", const std::string remark = "", const char* clientOid = "") { return withdraw(coin, address.c_str(), chain, amount, cb, tag, remark.c_str(), clientOid); }
    result<withdraw_res_t> withdraw(const char* coin, const std::string &address, const char* chain, double_type amount, withdraw_cb cb = {}, const char* tag = "", const char* remark = "", const std::string clientOid = "") { return withdraw(coin, address.c_str(), chain, amount, cb, tag, remark, clientOid.c_str()); }
    result<withdraw_res_t> withdraw(const char* coin, const std::string &address, const char* chain, double_type amount, withdraw_cb cb = {}, const char* tag = "", const char* remark = "", const char* clientOid = "") { return withdraw(coin, address.c_str(), chain, amount, cb, tag, remark, clientOid); }
    result<withdraw_res_t> withdraw(const char* coin, const char* address, const std::string &chain, double_type amount, withdraw_cb cb = {}, const std::string tag = "", const std::string remark = "", const std::string clientOid = "") { return withdraw(coin, address, chain.c_str(), amount, cb, tag.c_str(), remark.c_str(), clientOid.c_str()); }
    result<withdraw_res_t> withdraw(const char* coin, const char* address, const std::string &chain, double_type amount, withdraw_cb cb = {}, const std::string tag = "", const std::string remark = "", const char* clientOid = "") { return withdraw(coin, address, chain.c_str(), amount, cb, tag.c_str(), remark.c_str(), clientOid); }
    result<withdraw_res_t> withdraw(const char* coin, const char* address, const std::string &chain, double_type amount, withdraw_cb cb = {}, const std::string tag = "", const char* remark = "", const std::string clientOid = "") { return withdraw(coin, address, chain.c_str(), amount, cb, tag.c_str(), remark, clientOid.c_str()); }
    result<withdraw_res_t> withdraw(const char* coin, const char* address, const std::string &chain, double_type amount, withdraw_cb cb = {}, const std::string tag = "", const char* remark = "", const char* clientOid = "") { return withdraw(coin, address, chain.c_str(), amount, cb, tag.c_str(), remark, clientOid); }
    result<withdraw_res_t> withdraw(const char* coin, const char* address, const std::string &chain, double_type amount, withdraw_cb cb = {}, const char* tag = "", const std::string remark = "", const std::string clientOid = "") { return withdraw(coin, address, chain.c_str(), amount, cb, tag, remark.c_str(), clientOid.c_str()); }
    result<withdraw_res_t> withdraw(const char* coin, const char* address, const std::string &chain, double_type amount, withdraw_cb cb = {}, const char* tag = "", const std::string remark = "", const char* clientOid = "") { return withdraw(coin, address, chain.c_str(), amount, cb, tag, remark.c_str(), clientOid); }
    result<withdraw_res_t> withdraw(const char* coin, const char* address, const std::string &chain, double_type amount, withdraw_cb cb = {}, const char* tag = "", const char* remark = "", const std::string clientOid = "") { return withdraw(coin, address, chain.c_str(), amount, cb, tag, remark, clientOid.c_str()); }
    result<withdraw_res_t> withdraw(const char* coin, const char* address, const std::string &chain, double_type amount, withdraw_cb cb = {}, const char* tag = "", const char* remark = "", const char* clientOid = "") { return withdraw(coin, address, chain.c_str(), amount, cb, tag, remark, clientOid); }
    result<withdraw_res_t> withdraw(const char* coin, const char* address, const char* chain, double_type amount, withdraw_cb cb = {}, const std::string tag = "", const std::string remark = "", const std::string clientOid = "") { return withdraw(coin, address, chain, amount, cb, tag.c_str(), remark.c_str(), clientOid.c_str()); }
    result<withdraw_res_t> withdraw(const char* coin, const char* address, const char* chain, double_type amount, withdraw_cb cb = {}, const std::string tag = "", const std::string remark = "", const char* clientOid = "") { return withdraw(coin, address, chain, amount, cb, tag.c_str(), remark.c_str(), clientOid); }
    result<withdraw_res_t> withdraw(const char* coin, const char* address, const char* chain, double_type amount, withdraw_cb cb = {}, const std::string tag = "", const char* remark = "", const std::string clientOid = "") { return withdraw(coin, address, chain, amount, cb, tag.c_str(), remark, clientOid.c_str()); }
    result<withdraw_res_t> withdraw(const char* coin, const char* address, const char* chain, double_type amount, withdraw_cb cb = {}, const std::string tag = "", const char* remark = "", const char* clientOid = "") { return withdraw(coin, address, chain, amount, cb, tag.c_str(), remark, clientOid); }
    result<withdraw_res_t> withdraw(const char* coin, const char* address, const char* chain, double_type amount, withdraw_cb cb = {}, const char* tag = "", const std::string remark = "", const std::string clientOid = "") { return withdraw(coin, address, chain, amount, cb, tag, remark.c_str(), clientOid.c_str()); }
    result<withdraw_res_t> withdraw(const char* coin, const char* address, const char* chain, double_type amount, withdraw_cb cb = {}, const char* tag = "", const std::string remark = "", const char* clientOid = "") { return withdraw(coin, address, chain, amount, cb, tag, remark.c_str(), clientOid); }
    result<withdraw_res_t> withdraw(const char* coin, const char* address, const char* chain, double_type amount, withdraw_cb cb = {}, const char* tag = "", const char* remark = "", const std::string clientOid = "") { return withdraw(coin, address, chain, amount, cb, tag, remark, clientOid.c_str()); }
    result<withdraw_res_t> withdraw(const char* coin, const char* address, const char* chain, double_type amount, withdraw_cb cb = {}, const char* tag = "", const char* remark = "", const char* clientOid = "");

    // https://bitgetlimited.github.io/apidoc/en/spot/#inner-withdraw
    using withdraw_cb = std::function<bool(const char *fl, int ec, std::string errmsg, withdraw_res_t res)>;
    result<withdraw_res_t> innerWithdraw(const std::string &coin, const std::string &toUid, double_type amount, withdraw_cb cb = {}, std::string clientOid = "") { return innerWithdraw(coin.c_str(), toUid.c_str(), amount, cb, clientOid.c_str()); }
    result<withdraw_res_t> innerWithdraw(const std::string &coin, const std::string &toUid, double_type amount, withdraw_cb cb = {}, const char* clientOid = "") { return innerWithdraw(coin.c_str(), toUid.c_str(), amount, cb, clientOid); }
    result<withdraw_res_t> innerWithdraw(const std::string &coin, const char* toUid, double_type amount, withdraw_cb cb = {}, std::string clientOid = "") { return innerWithdraw(coin.c_str(), toUid, amount, cb, clientOid.c_str()); }
    result<withdraw_res_t> innerWithdraw(const std::string &coin, const char* toUid, double_type amount, withdraw_cb cb = {}, const char* clientOid = "") { return innerWithdraw(coin.c_str(), toUid, amount, cb, clientOid); }
    result<withdraw_res_t> innerWithdraw(const char* coin, const std::string &toUid, double_type amount, withdraw_cb cb = {}, std::string clientOid = "") { return innerWithdraw(coin, toUid.c_str(), amount, cb, clientOid.c_str()); }
    result<withdraw_res_t> innerWithdraw(const char* coin, const std::string &toUid, double_type amount, withdraw_cb cb = {}, const char* clientOid = "") { return innerWithdraw(coin, toUid.c_str(), amount, cb, clientOid); }
    result<withdraw_res_t> innerWithdraw(const char* coin, const char* toUid, double_type amount, withdraw_cb cb = {}, std::string clientOid = "") { return innerWithdraw(coin, toUid, amount, cb, clientOid.c_str()); }
    result<withdraw_res_t> innerWithdraw(const char* coin, const char* toUid, double_type amount, withdraw_cb cb = {}, const char* clientOid = "");

    // https://bitgetlimited.github.io/apidoc/en/spot/#get-withdraw-list
    using withdraw_list_cb = std::function<bool(const char *fl, int ec, std::string errmsg, deposit_withdrawals_t res)>;
    result<deposit_withdrawals_t> withdrawList(const std::string &coin, std::size_t startTime, std::size_t endTime, withdraw_list_cb cb = {}, std::size_t pageNo = 1, uint8_t pageSize = 20) { return withdrawList(coin.c_str(), startTime, endTime, cb, pageNo, pageSize); }
    result<deposit_withdrawals_t> withdrawList(const char* coin, std::size_t startTime, std::size_t endTime, withdraw_list_cb cb = {}, std::size_t pageNo = 1, uint8_t pageSize = 20);

    // https://bitgetlimited.github.io/apidoc/en/spot/#get-deposit-list
    using deposit_list_cb = std::function<bool(const char *fl, int ec, std::string errmsg, deposit_withdrawals_t res)>;
    result<deposit_withdrawals_t> depositList(const std::string &coin, std::size_t startTime, std::size_t endTime, deposit_list_cb cb = {}, std::size_t pageNo = 1, uint8_t pageSize = 20) { return depositList(coin.c_str(), startTime, endTime, cb, pageNo, pageSize); }
    result<deposit_withdrawals_t> depositList(const char* coin, std::size_t startTime, std::size_t endTime, deposit_list_cb cb = {}, std::size_t pageNo = 1, uint8_t pageSize = 20);

    // https://bitgetlimited.github.io/apidoc/en/spot/#get-apikey-info
    using apikey_cb = std::function<bool(const char *fl, int ec, std::string errmsg, apikey_t res)>;
    result<apikey_t> getApiKeyInfo(apikey_cb cb = {});

    // https://bitgetlimited.github.io/apidoc/en/spot/#get-account
    using account_cb = std::function<bool(const char *fl, int ec, std::string errmsg, spot_account_t res)>;
    result<spot_account_t> getSpotAccount(std::string coin, account_cb cb = {}) { return getSpotAccount(coin.c_str(), cb); }
    result<spot_account_t> getSpotAccount(const char* coin, account_cb cb = {});

    // https://bitgetlimited.github.io/apidoc/en/spot/#get-bills
    using bills_cb = std::function<bool(const char *fl, int ec, std::string errmsg, bills_t res)>;
    result<bills_t> getBills(std::size_t coinId, _group_type groupType, _biz_type bizType, bills_cb cb = {}, std::size_t after = 0, std::size_t before = 0, uint16_t limit = 100);

    // https://bitgetlimited.github.io/apidoc/en/spot/#get-transfer-list
    using transfers_cb = std::function<bool(const char *fl, int ec, std::string errmsg, transfers_t res)>;
    result<transfers_t> getTransferList(std::size_t coinId, std::string &fromType, transfers_cb cb = {}, std::size_t after = 0, std::size_t before = 0, uint16_t limit = 100) { return getTransferList(coinId, fromType.c_str(), cb, after, before, limit); }
    result<transfers_t> getTransferList(std::size_t coinId, const char* fromType, transfers_cb cb = {}, std::size_t after = 0, std::size_t before = 0, uint16_t limit = 100);

    // https://bitgetlimited.github.io/apidoc/en/spot/#place-order
    using place_order_cb = std::function<bool(const char *fl, int ec, std::string errmsg, spot_order_res_t res)>;
    result<spot_order_res_t> placeSpotOrder(const std::string &symbol, _side side, _order_type type, _force force, double_type quantity, place_order_cb cb = {}, double_type price = 0, std::string clientOid = "") { return placeSpotOrder(symbol.c_str(), side, type, force, quantity, cb, price, clientOid.c_str()); }
    result<spot_order_res_t> placeSpotOrder(const std::string &symbol, _side side, _order_type type, _force force, double_type quantity, place_order_cb cb = {}, double_type price = 0, const char* clientOid = "") { return placeSpotOrder(symbol.c_str(), side, type, force, quantity, cb, price, clientOid); }
    result<spot_order_res_t> placeSpotOrder(const char* symbol, _side side, _order_type type, _force force, double_type quantity, place_order_cb cb = {}, double_type price = 0, std::string clientOid = "") { return placeSpotOrder(symbol, side, type, force, quantity, cb, price, clientOid.c_str()); }
    result<spot_order_res_t> placeSpotOrder(const char* symbol, _side side, _order_type type, _force force, double_type quantity, place_order_cb cb = {}, double_type price = 0, const char* clientOid = "");

    // https://bitgetlimited.github.io/apidoc/en/spot/#batch-order
    using place_orders_cb = std::function<bool(const char *fl, int ec, std::string errmsg, spot_orders_res_t res)>;
    result<spot_orders_res_t> placeSpotOrders(const std::string &symbol, std::vector<_side> sides, std::vector<_order_type> types, std::vector<_force> forces, std::vector<double_type> quantities, std::vector<double_type> prices, std::vector<std::string> clientOids, place_orders_cb cb = {}) { return placeSpotOrders(symbol.c_str(), sides, types, forces, quantities, prices, clientOids, cb); }
    result<spot_orders_res_t> placeSpotOrders(const char* symbol, std::vector<_side> sides, std::vector<_order_type> types, std::vector<_force> forces, std::vector<double_type> quantities, std::vector<double_type> prices, std::vector<std::string> clientOids, place_orders_cb cb = {});

    // https://bitgetlimited.github.io/apidoc/en/spot/#cancel-order
    using cancel_order_cb = std::function<bool(const char *fl, int ec, std::string errmsg, spot_cancel_res_t res)>;
    result<spot_cancel_res_t> cancelSpotOrder(const std::string &symbol, const std::string &orderId, cancel_order_cb cb = {}) { return cancelSpotOrder(symbol.c_str(), orderId.c_str(), cb); }
    result<spot_cancel_res_t> cancelSpotOrder(const std::string &symbol, const char* orderId, cancel_order_cb cb = {}) { return cancelSpotOrder(symbol.c_str(), orderId, cb);}
    result<spot_cancel_res_t> cancelSpotOrder(const char* symbol, const std::string &orderId, cancel_order_cb cb = {}) { return cancelSpotOrder(symbol, orderId.c_str(), cb); }
    result<spot_cancel_res_t> cancelSpotOrder(const char* symbol, const char* orderId, cancel_order_cb cb = {});

    // https://bitgetlimited.github.io/apidoc/en/spot/#cancel-order-in-batch-single-instruments
    using cancel_orders_cb = std::function<bool(const char *fl, int ec, std::string errmsg, spot_cancel_res_t res)>;
    result<spot_cancel_res_t> cancelSpotOrders(const std::string &symbol, std::vector<std::string> orderIds, cancel_orders_cb cb = {}) { return cancelSpotOrders(symbol.c_str(), orderIds, cb); }
    result<spot_cancel_res_t> cancelSpotOrders(const char* symbol, std::vector<std::string> orderIds, cancel_orders_cb cb = {});

    // https://bitgetlimited.github.io/apidoc/en/spot/#get-order-details
    using order_detail_cb = std::function<bool(const char *fl, int ec, std::string errmsg, spot_orders_t res)>;
    result<spot_orders_t> getOrderDetails(const std::string &symbol, const std::string &orderId, order_detail_cb cb = {}, std::string clientOrderId = "") { return getOrderDetails(symbol.c_str(), orderId.c_str(), cb, clientOrderId.c_str()); }
    result<spot_orders_t> getOrderDetails(const std::string &symbol, const std::string &orderId, order_detail_cb cb = {}, const char* clientOrderId = "") { return getOrderDetails(symbol.c_str(), orderId.c_str(), cb, clientOrderId); }
    result<spot_orders_t> getOrderDetails(const std::string &symbol, const char* orderId, order_detail_cb cb = {}, std::string clientOrderId = "") { return getOrderDetails(symbol.c_str(), orderId, cb, clientOrderId.c_str()); }
    result<spot_orders_t> getOrderDetails(const std::string &symbol, const char* orderId, order_detail_cb cb = {}, const char* clientOrderId = "") { return getOrderDetails(symbol.c_str(), orderId, cb, clientOrderId); }
    result<spot_orders_t> getOrderDetails(const char* symbol, const std::string &orderId, order_detail_cb cb = {}, std::string clientOrderId = "") { return getOrderDetails(symbol, orderId.c_str(), cb, clientOrderId.c_str()); }
    result<spot_orders_t> getOrderDetails(const char* symbol, const std::string &orderId, order_detail_cb cb = {}, const char* clientOrderId = "") { return getOrderDetails(symbol, orderId.c_str(), cb, clientOrderId); }
    result<spot_orders_t> getOrderDetails(const char* symbol, const char* orderId, order_detail_cb cb = {}, std::string clientOrderId = "") { return getOrderDetails(symbol, orderId, cb, clientOrderId.c_str()); }
    result<spot_orders_t> getOrderDetails(const char* symbol, const char* orderId, order_detail_cb cb = {}, const char* clientOrderId = "");

    // https://bitgetlimited.github.io/apidoc/en/spot/#get-order-list
    using order_list_cb = std::function<bool(const char *fl, int ec, std::string errmsg, spot_orders_t res)>;
    result<spot_orders_t> getOrderList(const std::string symbol = "", order_list_cb cb = {}) { return getOrderList(symbol.c_str(), cb); }
    result<spot_orders_t> getOrderList(const char* symbol = "", order_list_cb cb = {});

    // https://bitgetlimited.github.io/apidoc/en/spot/#get-order-history
    using order_history_cb = std::function<bool(const char *fl, int ec, std::string errmsg, spot_orders_t res)>;
    result<spot_orders_t> orderHistory(const std::string &symbol, order_history_cb cb = {}, std::size_t after = 0, std::size_t before = 0, uint32_t limit = 100) { return orderHistory(symbol.c_str(), cb, after, before, limit); }
    result<spot_orders_t> orderHistory(const char* symbol, order_history_cb cb = {}, std::size_t after = 0, std::size_t before = 0, uint32_t limit = 100);

    // https://bitgetlimited.github.io/apidoc/en/spot/#get-transaction-details
    using transactions_cb = std::function<bool(const char *fl, int ec, std::string errmsg, transactions_t res)>;
    result<transactions_t> getTransactions(const std::string &symbol, transactions_cb cb = {}, std::string orderId = "", std::size_t after = 0, std::size_t before = 0, uint32_t limit = 100) { return getTransactions(symbol.c_str(), cb, orderId, after, before, limit); }
    result<transactions_t> getTransactions(const std::string &symbol, transactions_cb cb = {}, const char* orderId = "", std::size_t after = 0, std::size_t before = 0, uint32_t limit = 100) { return getTransactions(symbol.c_str(), cb, orderId, after, before, limit); }
    result<transactions_t> getTransactions(const char* symbol, transactions_cb cb = {}, std::string orderId = "", std::size_t after = 0, std::size_t before = 0, uint32_t limit = 100) { return getTransactions(symbol, cb, orderId.c_str(), after, before, limit); }
    result<transactions_t> getTransactions(const char* symbol, transactions_cb cb = {}, const char* orderId = "", std::size_t after = 0, std::size_t before = 0, uint32_t limit = 100);

    // https://bitgetlimited.github.io/apidoc/en/spot/#place-plan-order
    using place_spot_plan_order_cb = std::function<bool(const char *fl, int ec, std::string errmsg, spot_plan_order_res_t res)>;
    result<spot_plan_order_res_t> placeSpotPlanOrder(
        const std::string &symbol,
        _side side,
        double_type triggerPrice,
        double_type size,
        _trigger_type triggerType,
        _order_type orderType,
        place_spot_plan_order_cb cb = {},
        double_type executePrice = 0,
        std::string clientOid = "",
        _force force = _force::_normal) {
            return placeSpotPlanOrder(
                symbol.c_str(),
                side,
                triggerPrice,
                size,
                triggerType,
                orderType,
                cb,
                executePrice,
                clientOid,
                force); }
    result<spot_plan_order_res_t> placeSpotPlanOrder(
        const std::string &symbol,
        _side side,
        double_type triggerPrice,
        double_type size,
        _trigger_type triggerType,
        _order_type orderType,
        place_spot_plan_order_cb cb = {},
        double_type executePrice = 0,
        const char* clientOid = "",
        _force force = _force::_normal) {
            return placeSpotPlanOrder(
                symbol.c_str(),
                side,
                triggerPrice,
                size,
                triggerType,
                orderType,
                cb,
                executePrice,
                clientOid,
                force); }
    result<spot_plan_order_res_t> placeSpotPlanOrder(
        const char* symbol,
        _side side,
        double_type triggerPrice,
        double_type size,
        _trigger_type triggerType,
        _order_type orderType,
        place_spot_plan_order_cb cb = {},
        double_type executePrice = 0,
        std::string clientOid = "",
        _force force = _force::_normal) {
            return placeSpotPlanOrder(
                symbol,
                side,
                triggerPrice,
                size,
                triggerType,
                orderType,
                cb,
                executePrice,
                clientOid.c_str(),
                force); }
    result<spot_plan_order_res_t> placeSpotPlanOrder(const char* symbol, _side side, double_type triggerPrice, double_type size, _trigger_type triggerType, _order_type orderType, place_spot_plan_order_cb cb = {}, double_type executePrice = 0, const char* clientOid = "", _force force = _force::_normal);

    // https://bitgetlimited.github.io/apidoc/en/spot/#modify-plan-order
    using modify_spot_plan_order_cb = std::function<bool(const char *fl, int ec, std::string errmsg, spot_plan_order_res_t res)>;
    result<spot_plan_order_res_t> modifySpotPlanOrder(const std::string &orderId, double_type triggerPrice, _order_type orderType, modify_spot_plan_order_cb cb = {}, double_type executePrice = 0, double_type size = 0) { return modifySpotPlanOrder(orderId.c_str(), triggerPrice, orderType, cb, executePrice, size); }
    result<spot_plan_order_res_t> modifySpotPlanOrder(const char* orderId, double_type triggerPrice, _order_type orderType, modify_spot_plan_order_cb cb = {}, double_type executePrice = 0, double_type size = 0);

    // https://bitgetlimited.github.io/apidoc/en/spot/#cancel-plan-order
    using cancel_spot_plan_order_cb = std::function<bool(const char *fl, int ec, std::string errmsg, cancel_spot_plan_order_res_t res)>;
    result<cancel_spot_plan_order_res_t> cancelSpotPlanOrder(const std::string &orderId, cancel_spot_plan_order_cb cb = {}) { return cancelSpotPlanOrder(orderId.c_str(), cb); }
    result<cancel_spot_plan_order_res_t> cancelSpotPlanOrder(const char* orderId, cancel_spot_plan_order_cb cb = {});

    // https://bitgetlimited.github.io/apidoc/en/spot/#get-current-plan-orders
    using spot_plan_orders_cb = std::function<bool(const char *fl, int ec, std::string errmsg, spot_plan_orders_t res)>;
    result<spot_plan_orders_t> getSpotPlanOrders(const std::string &symbol, spot_plan_orders_cb cb = {}, uint16_t pageSize = 20, std::string lastEndId = "") { return getSpotPlanOrders(symbol.c_str(), cb, pageSize, lastEndId.c_str()); }
    result<spot_plan_orders_t> getSpotPlanOrders(const std::string &symbol, spot_plan_orders_cb cb = {}, uint16_t pageSize = 20, const char* lastEndId = "") { return getSpotPlanOrders(symbol, cb, pageSize, lastEndId); }
    result<spot_plan_orders_t> getSpotPlanOrders(const char* symbol, spot_plan_orders_cb cb = {}, uint16_t pageSize = 20, std::string lastEndId = "") { return getSpotPlanOrders(symbol, cb, pageSize, lastEndId.c_str()); }
    result<spot_plan_orders_t> getSpotPlanOrders(const char* symbol, spot_plan_orders_cb cb = {}, uint16_t pageSize = 20, const char* lastEndId = "");

    // https://bitgetlimited.github.io/apidoc/en/spot/#get-history-plan-orders
    result<spot_plan_orders_t> getSpotPlanOrderHistory(std::string &symbol, std::size_t startTime, std::size_t endTime, spot_plan_orders_cb cb = {}, uint16_t pageSize = 20, std::string lastEndId = "") { return getSpotPlanOrderHistory(symbol.c_str(), startTime, endTime, cb, pageSize, lastEndId.c_str()); }
    result<spot_plan_orders_t> getSpotPlanOrderHistory(std::string &symbol, std::size_t startTime, std::size_t endTime, spot_plan_orders_cb cb = {}, uint16_t pageSize = 20, const char* lastEndId = "") { return getSpotPlanOrderHistory(symbol, startTime, endTime, cb, pageSize, lastEndId); }
    result<spot_plan_orders_t> getSpotPlanOrderHistory(const char* symbol, std::size_t startTime, std::size_t endTime, spot_plan_orders_cb cb = {}, uint16_t pageSize = 20, std::string lastEndId = "") { return getSpotPlanOrderHistory(symbol, startTime, endTime, cb, pageSize, lastEndId.c_str()); }
    result<spot_plan_orders_t> getSpotPlanOrderHistory(const char* symbol, std::size_t startTime, std::size_t endTime, spot_plan_orders_cb cb = {}, uint16_t pageSize = 20, const char* lastEndId = "");
    
/*************************************************************************************************/

    // https://bitgetlimited.github.io/apidoc/en/mix/#get-all-symbols
    using contracts_cb = std::function<bool(const char *fl, int ec, std::string errmsg, contracts_t res)>;
    result<contracts_t> getContracts(_product_type productType, contracts_cb cb = {});

    // https://bitgetlimited.github.io/apidoc/en/mix/#get-depth
    result<depth_t> getFuturesDepth(const std::string &symbol, depth_cb cb = {}, uint8_t limit = 100) { return getFuturesDepth(symbol.c_str(), cb, limit); }
    result<depth_t> getFuturesDepth(const char* symbol, depth_cb cb = {}, uint8_t limit = 100);

    // https://bitgetlimited.github.io/apidoc/en/mix/#get-single-symbol-ticker
    using futures_ticker_cb = std::function<bool(const char *fl, int ec, std::string errmsg, futures_ticker_t res)>;
    result<futures_ticker_t> getFuturesTicker(const std::string &symbol, futures_ticker_cb cb = {}) { return getFuturesTicker(symbol.c_str(), cb); }
    result<futures_ticker_t> getFuturesTicker(const char* symbol, futures_ticker_cb cb = {});

    // https://bitgetlimited.github.io/apidoc/en/mix/#get-all-symbol-ticker
    using futures_tickers_cb = std::function<bool(const char *fl, int ec, std::string errmsg, futures_tickers_t res)>;
    result<futures_tickers_t> getFuturesTickers(_product_type productType, futures_tickers_cb cb = {});

    // https://bitgetlimited.github.io/apidoc/en/mix/#get-fills
    using fills_cb = std::function<bool(const char *fl, int ec, std::string errmsg, fills_t res)>;
    result<fills_t> getFills(const std::string &symbol, fills_cb cb = {}, uint16_t limit = 100) { return getFills(symbol.c_str(), cb, limit); }
    result<fills_t> getFills(const char* symbol, fills_cb cb = {}, uint16_t limit = 100);

    // https://bitgetlimited.github.io/apidoc/en/mix/#get-candle-data
    result<candles_t> getFuturesCandles(const std::string &symbol, _candle_gran granularity, std::size_t startTime, std::size_t endTime, candles_cb cb = {}) { return getFuturesCandles(symbol.c_str(), granularity, startTime, endTime, cb); }
    result<candles_t> getFuturesCandles(const char* symbol, _candle_gran granularity, std::size_t startTime, std::size_t endTime, candles_cb cb = {});

    // https://bitgetlimited.github.io/apidoc/en/mix/#get-symbol-index-price
    using index_cb = std::function<bool(const char *fl, int ec, std::string errmsg, index_t res)>;
    result<index_t> getFuturesIndexPrice(const std::string &symbol, index_cb cb = {}) { return getFuturesIndexPrice(symbol.c_str(), cb); }
    result<index_t> getFuturesIndexPrice(const char* symbol, index_cb cb = {});

    // https://bitgetlimited.github.io/apidoc/en/mix/#get-symbol-next-funding-time
    using funding_cb = std::function<bool(const char *fl, int ec, std::string errmsg, funding_t res)>;
    result<funding_t> getNextFundingTime(const std::string &symbol, funding_cb cb = {}) { return getNextFundingTime(symbol.c_str(), cb); }
    result<funding_t> getNextFundingTime(const char* symbol, funding_cb cb = {});

    // https://bitgetlimited.github.io/apidoc/en/mix/#get-history-funding-rate
    using history_funding_cb = std::function<bool(const char *fl, int ec, std::string errmsg, fundings_t res)>;
    result<fundings_t> getHistoryFundRate(const std::string &symbol, history_funding_cb cb = {}, uint16_t pageSize = 20, uint16_t pageNo = 1, bool nextPage = false) { return getHistoryFundRate(symbol.c_str(), cb, pageSize, pageNo, nextPage); }
    result<fundings_t> getHistoryFundRate(const char* symbol, history_funding_cb cb = {}, uint16_t pageSize = 20, uint16_t pageNo = 1, bool nextPage = false);

    // https://bitgetlimited.github.io/apidoc/en/mix/#get-current-funding-rate
    result<funding_t> getFundRate(const std::string &symbol, funding_cb cb = {}) { return getFundRate(symbol.c_str(), cb); }
    result<funding_t> getFundRate(const char* symbol, funding_cb cb = {});

    // https://bitgetlimited.github.io/apidoc/en/mix/#get-open-interest
    using open_interest_cb = std::function<bool(const char *fl, int ec, std::string errmsg, open_interest_t res)>;
    result<open_interest_t> getOpenInterest(const std::string &symbol, open_interest_cb cb = {}) { return getOpenInterest(symbol.c_str(), cb); }
    result<open_interest_t> getOpenInterest(const char* symbol, open_interest_cb cb = {});

    // https://bitgetlimited.github.io/apidoc/en/mix/#get-symbol-mark-price
    using mark_price_cb = std::function<bool(const char *fl, int ec, std::string errmsg, mark_price_t res)>;
    result<mark_price_t> getMarkPrice(const std::string &symbol, mark_price_cb cb = {}) { return getMarkPrice(symbol.c_str(), cb); }
    result<mark_price_t> getMarkPrice(const char* symbol, mark_price_cb cb = {});

    // https://bitgetlimited.github.io/apidoc/en/mix/#get-symbol-leverage
    using leverage_cb = std::function<bool(const char *fl, int ec, std::string errmsg, leverage_t res)>;
    result<leverage_t> getLeverage(const std::string &symbol, leverage_cb cb = {}) { return getLeverage(symbol.c_str(), cb); }
    result<leverage_t> getLeverage(const char* symbol, leverage_cb cb = {});

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
