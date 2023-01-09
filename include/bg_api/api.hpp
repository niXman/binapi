
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
    using spot_candles_cb = std::function<bool(const char *fl, int ec, std::string errmsg, candles_t res)>;
    result<candles_t> getSpotCandles(const std::string &symbol, const std::string &period, spot_candles_cb cb = {}, std::size_t startTime = 0, std::size_t endTime = 0, uint16_t limit = 100) { return getSpotCandles(symbol.c_str(), candle_gran_from_string(period), cb, startTime, endTime, limit); }
    result<candles_t> getSpotCandles(const std::string &symbol, const char* period, spot_candles_cb cb = {}, std::size_t startTime = 0, std::size_t endTime = 0, uint16_t limit = 100) { return getSpotCandles(symbol.c_str(), candle_gran_from_string(period), cb, startTime, endTime, limit); }
    result<candles_t> getSpotCandles(const std::string &symbol, std::size_t period, spot_candles_cb cb = {}, std::size_t startTime = 0, std::size_t endTime = 0, uint16_t limit = 100) { return getSpotCandles(symbol.c_str(), static_cast<_candle_gran>(period), cb, startTime, endTime, limit); }
    result<candles_t> getSpotCandles(const std::string &symbol, _candle_gran period, spot_candles_cb cb = {}, std::size_t startTime = 0, std::size_t endTime = 0, uint16_t limit = 100) { return getSpotCandles(symbol.c_str(), period, cb, startTime, endTime, limit); }
    result<candles_t> getSpotCandles(const char* symbol, const std::string &period, spot_candles_cb cb = {}, std::size_t startTime = 0, std::size_t endTime = 0, uint16_t limit = 100) { return getSpotCandles(symbol, candle_gran_from_string(period), cb, startTime, endTime, limit); }
    result<candles_t> getSpotCandles(const char* symbol, const char* period, spot_candles_cb cb = {}, std::size_t startTime = 0, std::size_t endTime = 0, uint16_t limit = 100) { return getSpotCandles(symbol, candle_gran_from_string(period), cb, startTime, endTime, limit); }
    result<candles_t> getSpotCandles(const char* symbol, std::size_t period, spot_candles_cb cb = {}, std::size_t startTime = 0, std::size_t endTime = 0, uint16_t limit = 100) { return getSpotCandles(symbol, static_cast<_candle_gran>(period), cb, startTime, endTime, limit); }
    result<candles_t> getSpotCandles(const char* symbol, _candle_gran period, spot_candles_cb cb = {}, std::size_t startTime = 0, std::size_t endTime = 0, uint16_t limit = 100);

    // https://bitgetlimited.github.io/apidoc/en/spot/#get-depth
    // Note that this doesn't support different step types due to the json returned having different value types
    // This may or may not be compensated in a future version
    using spot_depth_cb = std::function<bool(const char *fl, int ec, std::string errmsg, depth_t res)>;
    result<depth_t> getSpotDepth(const std::string &symbol, spot_depth_cb cb = {}, uint16_t limit = 100) { return getSpotDepth(symbol.c_str(), cb, limit); }
    result<depth_t> getSpotDepth(const char* symbol, spot_depth_cb cb = {}, uint16_t limit = 100);

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

    // https://bitgetlimited.github.io/apidoc/en/spot/#get-order-list

    // https://bitgetlimited.github.io/apidoc/en/spot/#get-order-history
    using order_history_cb = std::function<bool(const char *fl, int ec, std::string errmsg, spot_orders_t res)>;
    result<spot_orders_t> orderHistory(const std::string &symbol, order_history_cb cb = {}, std::size_t after = 0, std::size_t before = 0, uint32_t limit = 100) { return orderHistory(symbol.c_str(), cb, after, before, limit); }
    result<spot_orders_t> orderHistory(const char* symbol, order_history_cb cb = {}, std::size_t after = 0, std::size_t before = 0, uint32_t limit = 100);

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
