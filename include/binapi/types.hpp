
// ----------------------------------------------------------------------------
//                              Apache License
//                        Version 2.0, January 2004
//                     http://www.apache.org/licenses/
//
// This file is part of binapi(https://github.com/niXman/binapi) project.
//
// Copyright (c) 2019-2021 niXman (github dot nixman dog pm.me). All rights reserved.
// ----------------------------------------------------------------------------

#ifndef __binapi__types_hpp
#define __binapi__types_hpp

#include "double_type.hpp"
#include "enums.hpp"

#include <boost/variant.hpp>

#include <string>
#include <vector>
#include <map>
#include <cstdint>
#include <cassert>

// forward
namespace flatjson {
struct fjson;
} // ns flatjson

namespace binapi {

/*************************************************************************************************/

namespace rest {

struct ping_t {
    bool ok;

    static ping_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const ping_t &f);
};

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/rest-api.md#check-server-time
struct server_time_t {
    std::size_t serverTime;

    static server_time_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const server_time_t &f);
};

// https://github.com/binance/binance-spot-api-docs/blob/master/rest-api.md#current-average-price
struct avg_price_t {
    std::size_t mins;
    double_type price;

    static avg_price_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const avg_price_t &f);
};

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/rest-api.md#symbol-price-ticker
struct prices_t {
    struct price_t {
        std::string symbol;
        double_type price;

        static price_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const price_t &f);
    };

    static prices_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const prices_t &f);

    std::map<std::string, price_t> prices;

    bool is_valid_symbol(const std::string &sym) const
        { return is_valid_symbol(sym.c_str()); }
    bool is_valid_symbol(const char *sym) const;

    const price_t& get_by_symbol(const std::string &sym) const
        { return get_by_symbol(sym.c_str()); }
    const price_t& get_by_symbol(const char *sym) const;
};

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/rest-api.md#24hr-ticker-price-change-statistics
struct _24hrs_tickers_t {
    struct _24hrs_ticker_t {
        std::string symbol;
        double_type priceChange;
        double_type priceChangePercent;
        double_type weightedAvgPrice;
        double_type prevClosePrice;
        double_type lastPrice;
        double_type lastQty;
        double_type bidPrice;
        double_type askPrice;
        double_type openPrice;
        double_type highPrice;
        double_type lowPrice;
        double_type volume;
        double_type quoteVolume;
        std::size_t openTime;
        std::size_t closeTime;
        std::size_t firstId;
        std::size_t lastId;
        std::size_t count;

        static _24hrs_ticker_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const _24hrs_ticker_t &f);
    };

    std::map<std::string, _24hrs_ticker_t> tickers;

    static _24hrs_tickers_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const _24hrs_tickers_t &f);
};

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/rest-api.md#account-information-user_data
struct account_info_t {
    std::size_t makerCommission;
    std::size_t takerCommission;
    std::size_t buyerCommission;
    std::size_t sellerCommission;
    bool canTrade;
    bool canWithdraw;
    bool canDeposit;
    std::size_t updateTime;

    struct balance_t {
        std::string asset;
        double_type free;
        double_type locked;

        static balance_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const balance_t &f);
    };
    std::map<std::string, balance_t> balances;

    const balance_t& get_balance(const std::string &symbol) const
        { return get_balance(symbol.c_str()); }
    const balance_t& get_balance(const char *symbol) const;

    const double_type& add_balance(const std::string &symbol, const double_type &amount)
    { return add_balance(symbol.c_str(), amount); }
    const double_type& add_balance(const char *symbol, const double_type &amount);

    const double_type& sub_balance(const std::string &symbol, const double_type &amount)
    { return sub_balance(symbol.c_str(), amount); }
    const double_type& sub_balance(const char *symbol, const double_type &amount);

    static account_info_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const account_info_t &f);
};

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/rest-api.md#exchange-information
struct exchange_info_t {
    std::string timezone;
    std::size_t serverTime;
    std::vector<std::string> exchangeFilters;

    struct rate_limit_t {
        std::string rateLimitType;
        std::string interval;
        std::size_t limit;

        friend std::ostream &operator<<(std::ostream &os, const rate_limit_t &f);
    };
    std::vector<rate_limit_t> rateLimits;

    struct symbol_t {
        std::string symbol;
        std::string status;
        std::string baseAsset;
        std::size_t baseAssetPrecision;
        std::string quoteAsset;
        std::size_t quotePrecision;
        std::vector<std::string> orderTypes;
        bool icebergAllowed;
        bool ocoAllowed;
        bool quoteOrderQtyMarketAllowed;
        bool allowTrailingStop;
        bool cancelReplaceAllowed;

        struct filter_t {
            struct price_t {
                double_type minPrice;
                double_type maxPrice;
                double_type tickSize;

                friend std::ostream &operator<<(std::ostream &os, const price_t &f);
            };
            struct percent_price_t {
                double_type multiplierUp;
                double_type multiplierDown;
                std::size_t avgPriceMins;

                friend std::ostream &operator<<(std::ostream &os, const percent_price_t &f);
            };
            struct percent_price_by_side_t {
                double_type bidMultiplierUp;
                double_type bidMultiplierDown;
                double_type askMultiplierUp;
                double_type askMultiplierDown;
                std::size_t avgPriceMins;
            };
            struct lot_size_t {
                double_type minQty;
                double_type maxQty;
                double_type stepSize;

                friend std::ostream &operator<<(std::ostream &os, const lot_size_t &f);
            };
            struct market_lot_size_t {
                double_type minQty;
                double_type maxQty;
                double_type stepSize;

                friend std::ostream &operator<<(std::ostream &os, const market_lot_size_t &f);
            };
            struct min_notional_t {
                double_type minNotional;

                friend std::ostream &operator<<(std::ostream &os, const min_notional_t &f);
            };
            struct iceberg_parts_t {
                std::size_t limit;

                friend std::ostream &operator<<(std::ostream &os, const iceberg_parts_t &f);
            };
            struct max_num_orders_t {
                std::size_t maxNumOrders;

                friend std::ostream &operator<<(std::ostream &os, const max_num_orders_t &f);
            };

            struct max_num_algo_orders_t {
                std::size_t maxNumAlgoOrders;

                friend std::ostream &operator<<(std::ostream &os, const max_num_algo_orders_t &f);
            };

            struct max_position_t {
                double_type maxPosition;

                friend std::ostream &operator<<(std::ostream &os, const max_position_t &f);
            };

            struct trailing_delta_t {
                std::size_t minTrailingAboveDelta;
                std::size_t maxTrailingAboveDelta;
                std::size_t minTrailingBelowDelta;
                std::size_t maxTrailingBelowDelta;

                friend std::ostream &operator<<(std::ostream &os, const trailing_delta_t &f);
            };

            struct notional_t {
                double_type minNotional;
                bool applyMinToMarket;
                double_type maxNotional;
                bool applyMaxToMarket;
                std::size_t avgPriceMins;

                friend std::ostream &operator<<(std::ostream &os, const notional_t &f);
            };

            std::string filterType;
            boost::variant<
                 price_t
                ,percent_price_t
                ,percent_price_by_side_t
                ,lot_size_t
                ,market_lot_size_t
                ,min_notional_t
                ,iceberg_parts_t
                ,max_num_orders_t
                ,max_num_algo_orders_t
                ,max_position_t
                ,trailing_delta_t
                ,notional_t
            > filter;

            friend std::ostream &operator<<(std::ostream &os, const filter_t &f);
        };
        std::vector<filter_t> filters;

        template<typename T>
        const T& get_filter() const {
            for ( const auto &it: filters ) {
                const T *p = boost::get<T>(&it.filter);
                if ( p ) {
                    return *p;
                }
            }

            assert("bad T type" == nullptr);
        }

        const filter_t::price_t& get_filter_price() const
        { return get_filter<filter_t::price_t>(); }
        const filter_t::percent_price_t& get_filter_percent_price() const
        { return get_filter<filter_t::percent_price_t>(); }
        const filter_t::percent_price_by_side_t& get_filter_percent_price_by_side() const
        { return get_filter<filter_t::percent_price_by_side_t>(); }
        const filter_t::notional_t& get_filter_notional() const
        { return get_filter<filter_t::notional_t>(); }
        const filter_t::lot_size_t& get_filter_lot_size() const
        { return get_filter<filter_t::lot_size_t>(); }
        const filter_t::market_lot_size_t& get_filter_market_lot_size() const
        { return get_filter<filter_t::market_lot_size_t>(); }
        const filter_t::min_notional_t& get_filter_min_notional() const
        { return get_filter<filter_t::min_notional_t>(); }
        const filter_t::iceberg_parts_t& get_filter_iceberg_parts() const
        { return get_filter<filter_t::iceberg_parts_t>(); }
        const filter_t::max_num_orders_t& get_filter_max_num_orders() const
        { return get_filter<filter_t::max_num_orders_t>(); }
        const filter_t::max_num_algo_orders_t& get_filter_max_num_algo_orders() const
        { return get_filter<filter_t::max_num_algo_orders_t>(); }
        const filter_t::max_position_t& get_filter_max_position() const
        { return get_filter<filter_t::max_position_t>(); }
        const filter_t::trailing_delta_t& get_filter_trailing_delta() const
        { return get_filter<filter_t::trailing_delta_t>(); }

        friend std::ostream &operator<<(std::ostream &os, const symbol_t &s);
    };

    std::map<std::string, symbol_t> symbols;
    std::size_t permissions;

    bool is_valid_symbol(const std::string &sym) const
        { return is_valid_symbol(sym.c_str()); }
    bool is_valid_symbol(const char *sym) const;

    const symbol_t& get_by_symbol(const std::string &sym) const
        { return get_by_symbol(sym.c_str()); }
    const symbol_t& get_by_symbol(const char *sym) const;

    static exchange_info_t construct(const flatjson::fjson &json);
    friend std::ostream& operator<<(std::ostream &os, const exchange_info_t &s);
};

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/rest-api.md#order-book
struct depths_t {
    struct depth_t {
        double_type price;
        double_type amount;

        friend std::ostream &operator<<(std::ostream &os, const depth_t &s);
    };

    std::size_t lastUpdateId;
    std::vector<depth_t> bids;
    std::vector<depth_t> asks;

    static depths_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const depths_t &s);
};

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/rest-api.md#recent-trades-list
struct trades_t {
    struct trade_t {
        std::size_t id;
        double_type price;
        double_type qty;
        std::size_t time;
        bool isBuyerMaker;
        bool isBestMatch;

        static trade_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const trade_t &s);
    };

    std::vector<trade_t> trades;

    static trades_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const trades_t &s);
};

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/rest-api.md#compressedaggregate-trades-list
struct agg_trades_t {
    struct agg_trade_t {
        std::size_t id;
        double_type price;
        double_type qty;
        std::size_t first_id;
        std::size_t last_id;
        std::size_t time;
        bool isBuyerMaker;
        bool isBestMatch;

        static agg_trade_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const agg_trade_t &s);
    };

    std::vector<agg_trade_t> trades;

    static agg_trades_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const agg_trades_t &s);
};

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/rest-api.md#klinecandlestick-data
struct klines_t {
    struct kline_t {
        std::size_t start_time;
        std::size_t end_time;
        double_type open;
        double_type high;
        double_type low;
        double_type close;
        double_type volume;
        double_type quote_volume;
        std::size_t num_trades;
        double_type taker_buy_base_vol;
        double_type taker_buy_quote_vol;

        friend std::ostream &operator<<(std::ostream &os, const kline_t &s);
    };

    std::vector<kline_t> klines;

    static klines_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const klines_t &s);
};

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/rest-api.md#query-order-user_data
struct order_info_t {
    std::string symbol;
    std::size_t orderId;
    std::string clientOrderId;
    double_type price;
    double_type origQty;
    double_type executedQty;
    double_type cummulativeQuoteQty;
    std::string status;
    std::string timeInForce;
    std::string type;
    std::string side;
    double_type stopPrice;
    double_type icebergQty;
    std::size_t time;
    std::size_t updateTime;
    bool isWorking;

    static order_info_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const order_info_t &o);
};

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/rest-api.md#current-open-orders-user_data
// https://github.com/binance-exchange/binance-official-api-docs/blob/master/rest-api.md#all-orders-user_data
struct orders_info_t {
    std::map<std::string, std::vector<order_info_t>> orders;

    static orders_info_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const orders_info_t &o);
};

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/rest-api.md#new-order--trade
struct new_order_info_ack_t {
    std::string symbol;
    std::size_t orderId;
    std::string clientOrderId;
    std::size_t transactTime;

    static new_order_info_ack_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const new_order_info_ack_t &o);
};

struct new_order_info_result_t {
    std::string symbol;
    std::size_t orderId;
    std::string clientOrderId;
    std::size_t transactTime;
    double_type price;
    double_type origQty;
    double_type executedQty;
    double_type cummulativeQuoteQty;
    std::string status;
    std::string timeInForce;
    std::string type;
    std::string side;

    static new_order_info_result_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const new_order_info_result_t &o);
};

struct new_order_info_full_t {
    std::string symbol;
    std::size_t orderId;
    std::string clientOrderId;
    std::size_t transactTime;
    double_type price;
    double_type origQty;
    double_type executedQty;
    double_type cummulativeQuoteQty;
    std::string status;
    std::string timeInForce;
    std::string type;
    std::string side;
    struct fill_part {
        double_type price;
        double_type qty;
        double_type commission;
        std::string commissionAsset;
    };
    std::vector<fill_part> fills;

    static double_type avg_price(const std::vector<fill_part> &parts);
    static double_type max_price(const std::vector<fill_part> &parts);
    static double_type sum_amount(const std::vector<fill_part> &parts);
    static double_type sum_commission(const std::vector<fill_part> &parts);

    static new_order_info_full_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const new_order_info_full_t &o);
};

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/rest-api.md#test-new-order-trade
struct new_test_order_info_t {
    bool ok;

    static new_test_order_info_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const new_test_order_info_t &o);
};

struct new_order_resp_type
    :boost::variant<
         new_order_info_ack_t
        ,new_order_info_result_t
        ,new_order_info_full_t
        ,new_test_order_info_t
    >
{
    // ctor inheritance
    using boost::variant<
         new_order_info_ack_t
        ,new_order_info_result_t
        ,new_order_info_full_t
        ,new_test_order_info_t
    >::variant;

    std::pair<e_trade_resp_type, const void *>
    get_response_type() const {
        if ( const auto *p = boost::get<new_order_info_ack_t>(this) ) {
            return {e_trade_resp_type::ACK, p};
        } else if ( const auto *p = boost::get<new_order_info_result_t>(this) ) {
            return {e_trade_resp_type::RESULT, p};
        } else if ( const auto *p = boost::get<new_order_info_full_t>(this) ) {
            return {e_trade_resp_type::FULL, p};
        } else if ( const auto *p = boost::get<new_test_order_info_t>(this) ) {
            return {e_trade_resp_type::TEST, p};
        }

        return {e_trade_resp_type::UNKNOWN, nullptr};
    }

    bool is_valid_response_type()  const { const auto r =  get_response_type(); return r.first != e_trade_resp_type::UNKNOWN; }
    bool is_ack_response_type()    const { const auto r =  get_response_type(); return r.first == e_trade_resp_type::ACK; }
    bool is_result_response_type() const { const auto r =  get_response_type(); return r.first == e_trade_resp_type::RESULT; }
    bool is_full_response_type()   const { const auto r =  get_response_type(); return r.first == e_trade_resp_type::FULL; }
    bool is_test_response_type()   const { const auto r =  get_response_type(); return r.first == e_trade_resp_type::TEST; }

    const new_order_info_ack_t& get_response_ack() const {
        const auto r =  get_response_type();
        assert(r.first == e_trade_resp_type::ACK);

        return *static_cast<const new_order_info_ack_t *>(r.second);
    }
    const new_order_info_result_t& get_response_result() const {
        const auto r =  get_response_type();
        assert(r.first == e_trade_resp_type::RESULT);

        return *static_cast<const new_order_info_result_t *>(r.second);
    }
    const new_order_info_full_t& get_response_full() const {
        const auto r =  get_response_type();
        assert(r.first == e_trade_resp_type::FULL);

        return *static_cast<const new_order_info_full_t *>(r.second);
    }
    const new_test_order_info_t& get_response_test() const {
        const auto r =  get_response_type();
        assert(r.first == e_trade_resp_type::TEST);

        return *static_cast<const new_test_order_info_t *>(r.second);
    }

    std::size_t get_order_id() const {
        const auto r =  get_response_type();
        assert(
            r.first == e_trade_resp_type::ACK ||
            r.first == e_trade_resp_type::RESULT ||
            r.first == e_trade_resp_type::FULL
        );

        switch ( r.first ) {
            case e_trade_resp_type::ACK: return static_cast<const new_order_info_ack_t *>(r.second)->orderId;
            case e_trade_resp_type::RESULT: return static_cast<const new_order_info_result_t *>(r.second)->orderId;
            case e_trade_resp_type::FULL: return static_cast<const new_order_info_full_t *>(r.second)->orderId;;
            default: break;
        }

        assert(!"unreachable");

        return 0u;
    }

    static new_order_resp_type construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const new_order_resp_type &o);
};

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/rest-api.md#cancel-order-trade
struct cancel_order_info_t {
    std::string symbol;
    std::size_t orderId;
    std::string origClientOrderId;
    std::string clientOrderId;
    double_type price;
    double_type origQty;
    double_type executedQty;
    double_type cummulativeQuoteQty;
    std::string status;
    std::string timeInForce;
    std::string type;
    std::string side;

    static cancel_order_info_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const cancel_order_info_t &o);
};

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/rest-api.md#account-trade-list-user_data
struct my_trades_info_t {
    struct my_trade_info_t {
        std::string symbol;
        std::size_t id;
        std::size_t orderId;
        double_type price;
        double_type qty;
        double_type commission;
        std::string commissionAsset;
        std::size_t time;
        bool isBuyer;
        bool isMaker;
        bool isBestMatch;

        static my_trade_info_t construct(const flatjson::fjson &json);
        friend std::ostream &operator<<(std::ostream &os, const my_trade_info_t &o);
    };

    std::vector<my_trade_info_t> trades;

    static my_trades_info_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const my_trades_info_t &o);
};

/*************************************************************************************************/

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/rest-api.md#start-user-data-stream-user_stream
struct start_user_data_stream_t {
    std::string listenKey;

    static start_user_data_stream_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const start_user_data_stream_t &o);
};

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/rest-api.md#keepalive-user-data-stream-user_stream
struct ping_user_data_stream_t {
    bool ok;

    static ping_user_data_stream_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const ping_user_data_stream_t &o);
};

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/rest-api.md#close-user-data-stream-user_stream
struct close_user_data_stream_t {
    bool ok;

    static close_user_data_stream_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const close_user_data_stream_t &o);
};

/*************************************************************************************************/

} // ns rest

/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/

namespace ws {

/*************************************************************************************************/

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/web-socket-streams.md#aggregate-trade-streams
struct agg_trade_t {
    std::string e; // Event type
    std::size_t E; // Event time
    std::string s; // Symbol
    std::size_t a; // Aggregate trade ID
    double_type p; // Price
    double_type q; // Quantity
    std::size_t f; // First trade ID
    std::size_t l; // Last trade ID
    std::size_t T; // Trade time
    bool m; // Is the buyer the market maker?
    bool M; // Ignore

    static agg_trade_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const agg_trade_t &o);
};

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/web-socket-streams.md#trade-streams
struct trade_t {
    std::size_t E; // Event time
    std::string s; // Symbol
    std::size_t t; // Trade ID
    double_type p; // Price
    double_type q; // Quantity
    std::size_t b; // Buyer order ID
    std::size_t a; // Seller order ID
    std::size_t T; // Trade time
    bool m; // Is the buyer the market maker?
    bool M; // Ignore

    static trade_t construct(const flatjson::fjson &json);
    friend std::ostream &operator<<(std::ostream &os, const trade_t &o);
};

/*************************************************************************************************/

// https://github.com/binance/binance-spot-api-docs/blob/master/web-socket-streams.md#partial-book-depth-streams
struct part_depths_t {
    struct depth_t {
        double_type price;
        double_type amount;

        friend std::ostream &operator<<(std::ostream &os, const depth_t &o);
    };

    std::vector<depth_t> a;
    std::vector<depth_t> b;

    static part_depths_t construct(const flatjson::fjson &json);
    friend std::ostream& operator<<(std::ostream &os, const part_depths_t &o);
};

/*************************************************************************************************/

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/web-socket-streams.md#diff-depth-stream
struct diff_depths_t {
    struct depth_t {
        double_type price;
        double_type amount;

        friend std::ostream &operator<<(std::ostream &os, const depth_t &o);
    };

    std::size_t E;
    std::string s;
    std::size_t u;
    std::size_t U;
    std::vector<depth_t> a;
    std::vector<depth_t> b;

    static diff_depths_t construct(const flatjson::fjson &json);
    friend std::ostream& operator<<(std::ostream &os, const diff_depths_t &o);
};

/*************************************************************************************************/

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/web-socket-streams.md#klinecandlestick-streams
struct kline_t {
    std::size_t E; // Event time
    std::string s; // Symbol
    std::size_t t; // Kline start time
    std::size_t T; // Kline close time
    std::string i; // Interval
    std::size_t f; // First trade ID
    std::size_t L; // Last trade ID
    double_type o; // Open price
    double_type c; // Close price
    double_type h; // High price
    double_type l; // Low price
    double_type v; // Base asset volume
    std::size_t n; // Number of trades
    bool        x; // Is this kline closed?
    double_type q; // Quote asset volume
    double_type V; // Taker buy base asset volume
    double_type Q; // Taker buy quote asset volume

    static kline_t construct(const flatjson::fjson &json);
    friend std::ostream& operator<<(std::ostream &os, const kline_t &o);
    friend bool ohlc_equal(const kline_t &l, const kline_t &r);
};

std::ostream& ohlc(std::ostream &os, const kline_t &o);

/*************************************************************************************************/

// https://github.com/binance/binance-spot-api-docs/blob/master/web-socket-streams.md#individual-symbol-mini-ticker-stream
struct mini_ticker_t {
    std::size_t E; // Event time
    std::string s; // Symbol
    double_type c; // Close price
    double_type o; // Open price
    double_type h; // High price
    double_type l; // Low price
    double_type v; // Total traded base asset volume
    double_type q; // Total traded quote asset volume

    static mini_ticker_t construct(const flatjson::fjson &json);
    friend std::ostream& operator<<(std::ostream &os, const mini_ticker_t &o);
};

// https://github.com/binance/binance-spot-api-docs/blob/master/web-socket-streams.md#all-market-mini-tickers-stream
struct mini_tickers_t {
    std::map<std::string, mini_ticker_t> tickers;

    static mini_tickers_t construct(const flatjson::fjson &json);
    friend std::ostream& operator<<(std::ostream &os, const mini_tickers_t &o);
};

/*************************************************************************************************/

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/web-socket-streams.md#individual-symbol-ticker-streams
struct market_ticker_t {
    std::size_t E; // Event time
    std::string s; // Symbol
    double_type p; // Price change
    double_type P; // Price change percent
    double_type w; // Weighted average price
    double_type x; // First trade(F)-1 price (first trade before the 24hr rolling window)
    double_type c; // Last price
    double_type Q; // Last quantity
    double_type b; // Best bid price
    double_type B; // Best bid quantity
    double_type a; // Best ask price
    double_type A; // Best ask quantity
    double_type o; // Open price
    double_type h; // High price
    double_type l; // Low price
    double_type v; // Total traded base asset volume
    double_type q; // Total traded quote asset volume
    std::size_t O; // Statistics open time
    std::size_t C; // Statistics close time
    std::size_t F; // First trade ID
    std::size_t L; // Last trade Id
    std::size_t n; // Total number of trades

    static market_ticker_t construct(const flatjson::fjson &json);
    friend std::ostream& operator<<(std::ostream &os, const market_ticker_t &o);
};

// https://github.com/binance-exchange/binance-official-api-docs/blob/master/web-socket-streams.md#all-market-tickers-stream
struct markets_tickers_t {
    std::map<std::string, market_ticker_t> tickers;

    static markets_tickers_t construct(const flatjson::fjson &json);
    friend std::ostream& operator<<(std::ostream &os, const markets_tickers_t &o);
};

/*************************************************************************************************/

// https://github.com/binance/binance-spot-api-docs/blob/master/web-socket-streams.md#individual-symbol-book-ticker-streams
struct book_ticker_t {
    std::size_t u;
    std::string s;
    double_type b;
    double_type B;
    double_type a;
    double_type A;

    static book_ticker_t construct(const flatjson::fjson &json);
    friend std::ostream& operator<<(std::ostream &os, const book_ticker_t &o);
};

/*************************************************************************************************/

} // ns ws

/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/

namespace userdata {

/*************************************************************************************************/

// https://github.com/binance/binance-spot-api-docs/blob/master/user-data-stream.md#account-update
struct account_update_t {
    struct balance_t {
        std::string a;
        double_type f;
        double_type l;

        friend std::ostream& operator<<(std::ostream &os, const balance_t &o);
    };

    std::string e;
    std::size_t E;
    std::size_t u;
    std::map<std::string, balance_t> B;

    static account_update_t construct(const flatjson::fjson &json);
    friend std::ostream& operator<<(std::ostream &os, const account_update_t &o);
};

/*************************************************************************************************/

// https://github.com/binance/binance-spot-api-docs/blob/master/user-data-stream.md#balance-update
struct balance_update_t {
    std::string e;
    std::size_t E;
    std::string a;
    double_type d;
    std::size_t T;

    static balance_update_t construct(const flatjson::fjson &json);
    friend std::ostream& operator<<(std::ostream &os, const balance_update_t &o);
};

/*************************************************************************************************/

// https://github.com/binance/binance-spot-api-docs/blob/master/user-data-stream.md#order-update
struct order_update_t {
    std::string e;
    std::size_t E;
    std::string s;
    std::string c;
    std::string S;
    std::string o;
    std::string f;
    double_type q;
    double_type p;
    double_type P;
    double_type F;
    std::string C;
    std::string x;
    std::string X;
    std::string r;
    std::size_t i;
    double_type l;
    double_type z;
    double_type L;
    double_type n;
    std::string N;
    std::size_t T;
    std::size_t t;
    std::size_t I;
    bool        w;
    bool        m;
    bool        M;
    std::size_t O;
    double_type Z;

    static order_update_t construct(const flatjson::fjson &json);
    friend std::ostream& operator<<(std::ostream &os, const order_update_t &o);
};

/*************************************************************************************************/

// wrapper for account_update_t and order_update_t
struct userdata_stream_t {
    std::string data;

    static userdata_stream_t construct(const flatjson::fjson &json);
    friend std::ostream& operator<<(std::ostream &os, const userdata_stream_t &o);
};

/*************************************************************************************************/

} // ns userdata

/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/

} // ns binapi

#endif // __binapi__types_hpp
