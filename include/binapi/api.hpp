
// ----------------------------------------------------------------------------
//                              Apache License
//                        Version 2.0, January 2004
//                     http://www.apache.org/licenses/
//
// This file is part of binapi(https://github.com/niXman/binapi) project.
//
// Copyright (c) 2019-2021 niXman (github dot nixman dog pm.me). All rights reserved.
// ----------------------------------------------------------------------------

#ifndef __binapi__api_hpp
#define __binapi__api_hpp

#include "types.hpp"
#include "enums.hpp"

#include <memory>
#include <functional>

namespace boost {
namespace asio {

class io_context;

} // ns asio
} // ns boost

namespace binapi {
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
         boost::asio::io_context &ioctx
        ,std::string host
        ,std::string port
        ,std::string pk
        ,std::string sk
        ,std::size_t timeout
        ,std::string client_api_string = "binapi-0.0.1"
    );
    virtual ~api();

    api(const api &) = delete;
    api(api &&) = default;

    // https://github.com/binance/binance-spot-api-docs/blob/master/rest-api.md#test-connectivity
    using ping_cb = std::function<bool(const char *fl, int ec, std::string errmsg, ping_t res)>;
    result<ping_t>
    ping(ping_cb cb = {});

    // https://github.com/binance/binance-spot-api-docs/blob/master/rest-api.md#check-server-time
    using server_time_cb = std::function<bool(const char *fl, int ec, std::string errmsg, server_time_t res)>;
    result<server_time_t>
    server_time(server_time_cb cb = {});

    // https://github.com/binance/binance-spot-api-docs/blob/master/rest-api.md#exchange-information
    using exchange_info_cb = std::function<bool(const char *fl, int ec, std::string errmsg, exchange_info_t res)>;
    result<exchange_info_t>
    exchange_info(exchange_info_cb cb = {});
    result<exchange_info_t>
    exchange_info(const char *symbol, exchange_info_cb cb = {});
    result<exchange_info_t>
    exchange_info(const std::string &symbol, exchange_info_cb cb = {}) { return exchange_info(symbol.c_str(), std::move(cb)); }
    result<exchange_info_t>
    exchange_info(const std::vector<std::string> &symbols, exchange_info_cb cb = {});

    // https://github.com/binance/binance-spot-api-docs/blob/master/rest-api.md#order-book
    using depths_cb = std::function<bool(const char *fl, int ec, std::string errmsg, depths_t res)>;
    result<depths_t>
    depths(const std::string &symbol, std::size_t limit, depths_cb cb = {}) { return depths(symbol.c_str(), limit, std::move(cb)); }
    result<depths_t>
    depths(const char *symbol, std::size_t limit, depths_cb cb = {});

    // https://github.com/binance/binance-spot-api-docs/blob/master/rest-api.md#recent-trades-list
    using trade_cb = std::function<bool(const char *fl, int ec, std::string errmsg, trades_t::trade_t res)>;
    result<trades_t::trade_t>
    trade(const std::string &symbol, trade_cb cb = {}) { return trade(symbol.c_str(), std::move(cb)); }
    result<trades_t::trade_t>
    trade(const char *symbol, trade_cb cb = {});

    using trades_cb = std::function<bool(const char *fl, int ec, std::string errmsg, trades_t res)>;
    result<trades_t>
    trades(const std::string &symbol, std::size_t limit, trades_cb cb = {}) { return trades(symbol.c_str(), limit, std::move(cb)); }
    result<trades_t>
    trades(const char *symbol, std::size_t limit, trades_cb cb = {});

    // https://github.com/binance/binance-spot-api-docs/blob/master/rest-api.md#symbol-price-ticker
    using price_cb = std::function<bool(const char *fl, int ec, std::string errmsg, prices_t::price_t res)>;
    result<prices_t::price_t>
    price(const std::string &symbol, price_cb cb = {}) { return price(symbol.c_str(), std::move(cb)); }
    result<prices_t::price_t>
    price(const char *symbol, price_cb cb = {});

    using prices_cb = std::function<bool(const char *fl, int ec, std::string errmsg, prices_t res)>;
    result<prices_t>
    prices(prices_cb cb = {});

    // https://github.com/binance/binance-spot-api-docs/blob/master/rest-api.md#current-average-price
    using avg_price_cb = std::function<bool(const char *fl, int ec, std::string errmsg, avg_price_t res)>;
    result<avg_price_t>
    avg_price(const char *symbol, avg_price_cb cb = {});

    // https://github.com/binance/binance-spot-api-docs/blob/master/rest-api.md#24hr-ticker-price-change-statistics
    using _24hrs_ticker_cb = std::function<bool(const char *fl, int ec, std::string errmsg, _24hrs_tickers_t::_24hrs_ticker_t res)>;
    result<_24hrs_tickers_t::_24hrs_ticker_t>
    _24hrs_ticker(const std::string &symbol, _24hrs_ticker_cb cb = {}) { return _24hrs_ticker(symbol.c_str(), std::move(cb)); }
    result<_24hrs_tickers_t::_24hrs_ticker_t>
    _24hrs_ticker(const char *symbol, _24hrs_ticker_cb cb = {});

    using _24hrs_tickers_cb = std::function<bool(const char *fl, int ec, std::string errmsg, _24hrs_tickers_t res)>;
    result<_24hrs_tickers_t>
    _24hrs_tickers(_24hrs_tickers_cb cb = {});

    // https://github.com/binance/binance-spot-api-docs/blob/master/rest-api.md#compressedaggregate-trades-list
    using agg_trade_cb = std::function<bool(const char *fl, int ec, std::string errmsg, agg_trades_t::agg_trade_t res)>;
    result<agg_trades_t::agg_trade_t>
    agg_trade(const std::string &symbol, agg_trade_cb cb = {}) { return agg_trade(symbol.c_str(), std::move(cb)); }
    result<agg_trades_t::agg_trade_t>
    agg_trade(const char *symbol, agg_trade_cb cb = {});

    using agg_trades_cb = std::function<bool(const char *fl, int ec, std::string errmsg, agg_trades_t res)>;
    result<agg_trades_t>
    agg_trades(const std::string &symbol, std::size_t limit, agg_trades_cb cb = {}) { return agg_trades(symbol.c_str(), limit, std::move(cb)); }
    result<agg_trades_t>
    agg_trades(const char *symbol, std::size_t limit, agg_trades_cb cb = {});

    // https://github.com/binance/binance-spot-api-docs/blob/master/rest-api.md#klinecandlestick-data
    using klines_cb = std::function<bool(const char *fl, int ec, std::string errmsg, klines_t res)>;
    result<klines_t>
    klines(const std::string &symbol, const std::string &interval, std::size_t limit, klines_cb cb = {}) { return klines(symbol.c_str(), interval.c_str(), limit, std::move(cb)); }
    result<klines_t>
    klines(const char *symbol, const char *interval, std::size_t limit, klines_cb cb = {});

    // https://github.com/binance/binance-spot-api-docs/blob/master/rest-api.md#account-information-user_data
    using account_info_cb = std::function<bool(const char *fl, int ec, std::string errmsg, account_info_t res)>;
    result<account_info_t>
    account_info(account_info_cb cb = {});

    // https://github.com/binance/binance-spot-api-docs/blob/master/rest-api.md#query-order-user_data
    using order_info_cb = std::function<bool(const char *fl, int ec, std::string errmsg, order_info_t res)>;
    result<order_info_t>
    order_info(const std::string &symbol, std::size_t orderid, const std::string &client_orderid = std::string{}, order_info_cb cb = {})
    { return order_info(symbol.c_str(), orderid, client_orderid.empty() ? nullptr : client_orderid.c_str(), std::move(cb)); }
    result<order_info_t>
    order_info(const char *symbol, std::size_t orderid, const char *client_orderid = nullptr, order_info_cb cb = {});

    // https://github.com/binance/binance-spot-api-docs/blob/master/rest-api.md#current-open-orders-user_data
    using open_orders_cb = std::function<bool(const char *fl, int ec, std::string errmsg, orders_info_t res)>;
    result<orders_info_t>
    open_orders(const std::string &symbol, open_orders_cb cb = {}) { return open_orders(symbol.c_str(), std::move(cb)); }
    result<orders_info_t>
    open_orders(const char *symbol, open_orders_cb cb = {});

    // https://github.com/binance/binance-spot-api-docs/blob/master/rest-api.md#all-orders-user_data
    using all_orders_cb = std::function<bool(const char *fl, int ec, std::string errmsg, orders_info_t res)>;
    result<orders_info_t>
    all_orders(
         const std::string &symbol
        ,std::size_t orderid = 0
        ,std::size_t start_time = 0
        ,std::size_t end_time = 0
        ,std::size_t limit = 0
        ,all_orders_cb cb = {}
    ) {
        return all_orders(
             symbol.empty() ? nullptr : symbol.c_str()
            ,orderid
            ,start_time
            ,end_time
            ,limit
            ,std::move(cb)
        );
    }
    result<orders_info_t>
    all_orders(
         const char *symbol
        ,std::size_t orderid = 0
        ,std::size_t start_time = 0
        ,std::size_t end_time = 0
        ,std::size_t limit = 0
        ,all_orders_cb cb = {}
    );

    // https://github.com/binance/binance-spot-api-docs/blob/master/rest-api.md#new-order--trade
    // NOTE: if 'ec' not zero - the 'res' arg is in undefined state.
    using new_order_cb = std::function<bool(const char *fl, int ec, std::string errmsg, new_order_resp_type res)>;
    result<new_order_resp_type>
    new_order(
         const std::string &symbol
        ,const e_side side
        ,const e_type type
        ,const e_time time
        ,const e_trade_resp_type resp
        ,const std::string &amount
        ,const std::string &price
        ,const std::string &client_order_id
        ,const std::string &stop_price
        ,const std::string &iceberg_amount
        ,new_order_cb cb = {}
    ) {
        return new_order(
             symbol.c_str()
            ,side
            ,type
            ,time
            ,resp
            ,(amount.empty() ? nullptr : amount.c_str())
            ,(price.empty() ? nullptr : price.c_str())
            ,(client_order_id.empty() ? nullptr : client_order_id.c_str())
            ,(stop_price.empty() ? nullptr : stop_price.c_str())
            ,(iceberg_amount.empty() ? nullptr : iceberg_amount.c_str())
            ,std::move(cb)
        );
    }
    result<new_order_resp_type>
    new_order(
         const char *symbol
        ,const e_side side
        ,const e_type type
        ,const e_time time
        ,const e_trade_resp_type resp
        ,const char *amount
        ,const char *price
        ,const char *client_order_id
        ,const char *stop_price
        ,const char *iceberg_amount
        ,new_order_cb cb = {}
    );

    // https://github.com/binance/binance-spot-api-docs/blob/master/rest-api.md#test-new-order-trade
    result<new_order_resp_type>
    new_test_order(
         const std::string &symbol
        ,const e_side side
        ,const e_type type
        ,const e_time time
        ,const e_trade_resp_type resp
        ,const std::string &amount
        ,const std::string &price
        ,const std::string &client_order_id
        ,const std::string &stop_price
        ,const std::string &iceberg_amount
        ,new_order_cb cb = {}
    ) {
        return new_test_order(
             symbol.c_str()
            ,side
            ,type
            ,time
            ,resp
            ,(amount.empty() ? nullptr : amount.c_str())
            ,(price.empty() ? nullptr : price.c_str())
            ,(client_order_id.empty() ? nullptr : client_order_id.c_str())
            ,(stop_price.empty() ? nullptr : stop_price.c_str())
            ,(iceberg_amount.empty() ? nullptr : iceberg_amount.c_str())
            ,std::move(cb)
        );
    }
    result<new_order_resp_type>
    new_test_order(
         const char *symbol
        ,const e_side side
        ,const e_type type
        ,const e_time time
        ,const e_trade_resp_type resp
        ,const char *amount
        ,const char *price
        ,const char *client_order_id
        ,const char *stop_price
        ,const char *iceberg_amount
        ,new_order_cb cb = {}
    );

    // https://github.com/binance/binance-spot-api-docs/blob/master/rest-api.md#cancel-order-trade
    using cancel_order_cb = std::function<bool(const char *fl, int ec, std::string errmsg, cancel_order_info_t res)>;
    result<cancel_order_info_t>
    cancel_order(
         const std::string &symbol
        ,std::size_t order_id
        ,const std::string &client_order_id
        ,const std::string &new_client_order_id
        ,cancel_order_cb cb = {}
    ) {
        return cancel_order(
             symbol.c_str()
            ,order_id
            ,(client_order_id.empty() ? nullptr : client_order_id.c_str())
            ,(new_client_order_id.empty() ? nullptr : new_client_order_id.c_str())
            ,std::move(cb)
        );
    }
    result<cancel_order_info_t>
    cancel_order(const char *symbol, std::size_t order_id, const char *client_order_id, const char *new_client_order_id, cancel_order_cb cb = {});

    // https://github.com/binance/binance-spot-api-docs/blob/master/rest-api.md#account-trade-list-user_data
    using my_trades_cb = std::function<bool(const char *fl, int ec, std::string errmsg, my_trades_info_t res)>;
    result<my_trades_info_t>
    my_trades(
         const std::string &symbol
        ,std::size_t start_time
        ,std::size_t end_time
        ,std::size_t from_id
        ,std::size_t limit
        ,my_trades_cb cb = {}
    )
    { return my_trades(symbol.c_str(), start_time, end_time, from_id, limit, std::move(cb)); }
    result<my_trades_info_t>
    my_trades(
         const char *symbol
        ,std::size_t start_time
        ,std::size_t end_time
        ,std::size_t from_id
        ,std::size_t limit
        ,my_trades_cb cb = {}
    );

    // https://github.com/binance/binance-spot-api-docs/blob/master/rest-api.md#start-user-data-stream-user_stream
    using start_user_data_stream_cb = std::function<bool(const char *fl, int ec, std::string errmsg, start_user_data_stream_t res)>;
    result<start_user_data_stream_t>
    start_user_data_stream(start_user_data_stream_cb cb = {});

    // https://github.com/binance/binance-spot-api-docs/blob/master/rest-api.md#keepalive-user-data-stream-user_stream
    using ping_user_data_stream_cb = std::function<bool(const char *fl, int ec, std::string errmsg, ping_user_data_stream_t res)>;
    result<ping_user_data_stream_t>
    ping_user_data_stream(const std::string &listen_key, ping_user_data_stream_cb cb = {}) { return ping_user_data_stream(listen_key.c_str(), std::move(cb)); }
    result<ping_user_data_stream_t>
    ping_user_data_stream(const char *listen_key, ping_user_data_stream_cb cb = {});

    // https://github.com/binance/binance-spot-api-docs/blob/master/rest-api.md#close-user-data-stream-user_stream
    using close_user_data_stream_cb = std::function<bool(const char *fl, int ec, std::string errmsg, close_user_data_stream_t res)>;
    result<close_user_data_stream_t>
    close_user_data_stream(const std::string &listen_key, close_user_data_stream_cb cb = {}) { return close_user_data_stream(listen_key.c_str(), std::move(cb)); }
    result<close_user_data_stream_t>
    close_user_data_stream(const char *listen_key, close_user_data_stream_cb cb = {});

private:
    struct impl;
    std::unique_ptr<impl> pimpl;
};

/*************************************************************************************************/

} // ns rest
} // ns binapi

#endif // __binapi__api_hpp
