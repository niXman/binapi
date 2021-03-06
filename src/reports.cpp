
// ----------------------------------------------------------------------------
//                              Apache License
//                        Version 2.0, January 2004
//                     http://www.apache.org/licenses/
//
// This file is part of binapi(https://github.com/niXman/binapi) project.
//
// Copyright (c) 2019-2021 niXman (github dot nixman dog pm.me). All rights reserved.
// ----------------------------------------------------------------------------

#include <binapi/reports.hpp>
#include <binapi/api.hpp>
#include <binapi/tools.hpp>
#include <binapi/pairslist.hpp>
#include <binapi/iofmt.hpp>
#include <binapi/dtf.hpp>

#include <boost/format.hpp>

#include <iostream> // TODO: comment out

namespace binapi {

/*************************************************************************************************/

std::uint64_t mstime_from_str(const std::string &strtime) {
    std::istringstream is(strtime);
    std::tm tm{};
    is >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
    assert(!is.fail());

    auto res = static_cast<std::uint64_t>(std::mktime(&tm));
    res *= 1000; // binance requires time in milliseconds

    auto p = strtime.rfind('.');
    if ( p != std::string::npos ) {
        std::size_t ms = std::strtoul(&strtime[p + 1], nullptr, 10);
        res += ms;
    }

    return res;
}

/*************************************************************************************************/

using trade_info_container_t = std::vector<rest::my_trades_info_t::my_trade_info_t>;
using order_info_container_t = std::vector<rest::order_info_t>;

/*************************************************************************************************/

std::vector<std::string> get_symbols_of_nonempty_balances(const binapi::rest::account_info_t &accinfo) {
    std::vector<std::string> res;

    for ( const auto &it: accinfo.balances ) {
        if ( it.second.free > 0 || it.second.locked > 0 ) {
            res.push_back(it.first);
        }
    }

    return res;
}

/*************************************************************************************************/

std::vector<std::string> get_pairs_for_pairs(
     const rest::account_info_t &accinfo
    ,const rest::exchange_info_t &exinfo
    ,const std::vector<std::string> &pairs)
{
    std::vector<std::string> mpairs;
    if ( pairs.empty() ) {
        std::vector<std::string> symbols = get_symbols_of_nonempty_balances(accinfo);
        for ( const auto &it: symbols ) {
            std::string filter = "*";
            filter += it;
            auto tmp0 = process_pairs(filter, "", exinfo);

            filter = it;
            filter += "*";
            auto tmp1 = process_pairs(filter, "", exinfo);

            mpairs.insert(mpairs.end(), tmp0.begin(), tmp0.end());
            mpairs.insert(mpairs.end(), tmp1.begin(), tmp1.end());
        }
    } else {
        for ( const auto &it: pairs ) {
            auto tmp = process_pairs(it, "", exinfo);
            mpairs.insert(mpairs.end(), tmp.begin(), tmp.end());
        }
    }

    std::sort(std::begin(mpairs), std::end(mpairs),
        [](std::string l, std::string r) {
            std::reverse(l.begin(), l.end());
            std::reverse(r.begin(), r.end());

            return l < r;
        }
    );
    mpairs.erase(std::unique(std::begin(mpairs), std::end(mpairs)), std::end(mpairs));

    return mpairs;
}

/*************************************************************************************************/

void remove_all_buy_trades(trade_info_container_t &trades) {
    trade_info_container_t tmp;

    for ( auto &it: trades ) {
        if ( !it.isBuyer ) {
            tmp.push_back(std::move(it));
        }
    }

    trades = std::move(tmp);
}

/*************************************************************************************************/

struct cycle_pair {
    rest::order_info_t sell;
    rest::order_info_t buy;
};

std::vector<cycle_pair> get_full_cycle_trades(
     binapi::rest::api &api
    ,const trade_info_container_t &trades
    ,const std::function<void(const rest::order_info_t &)> &tick)
{
    std::vector<cycle_pair> res;
    for ( auto &oit: trades ) {
        auto sell_order_info = api.order_info(oit.symbol, oit.orderId);
        assert(sell_order_info);

        if ( tick ) { tick(sell_order_info.v); }
        if ( sell_order_info.v.clientOrderId.empty() ) { continue; }
        if ( sell_order_info.v.status == "PARTIALLY_FILLED" ) { continue; }

        auto buy_order_id_ptr = std::strchr(sell_order_info.v.clientOrderId.c_str(), '-');
        if ( !buy_order_id_ptr ) { continue; }

        std::size_t buy_order_id = std::strtoul(buy_order_id_ptr+1, nullptr, 10);

        auto buy_order_info = api.order_info(oit.symbol, buy_order_id);
        assert(buy_order_info);

        if ( tick ) { tick(buy_order_info.v); }

        std::cout
        << (boost::format("%-10s: ") % oit.symbol)
        << "B:" << buy_order_info.v.orderId
        << " -> S:" << sell_order_info.v.orderId
        << " C:" << sell_order_info.v.clientOrderId
        << std::endl;

        cycle_pair cpair{
             std::move(sell_order_info.v)
            ,std::move(buy_order_info.v)
        };
        res.push_back(std::move(cpair));
    }

    return res;
}

/*************************************************************************************************/

struct pair_trades {
    std::size_t cycles;
    double_type total_buy_base;
    double_type total_buy_quoted;
    double_type total_sell_base;
    double_type total_sell_quoted;

    friend std::ostream& operator<< (std::ostream &os, const pair_trades &o) {
        os
        << "{"
        << "\"cycles\":" << o.cycles << ","
        << "\"total_buy_base\":\"" << o.total_buy_base << "\","
        << "\"total_buy_quoted\":\"" << o.total_buy_quoted << "\","
        << "\"total_sell_base\":\"" << o.total_sell_base << "\","
        << "\"total_sell_quoted\":\"" << o.total_sell_quoted << "\""
        << "}";

        return os;
    }
};

pair_trades calc_pair_trades_info(const std::vector<cycle_pair> &trades) {
    pair_trades res;
    res.cycles = trades.size();

    for ( const auto &it: trades ) {
        res.total_buy_base += it.buy.origQty;
        res.total_buy_quoted += it.buy.cummulativeQuoteQty;
        res.total_sell_base += it.sell.origQty;
        res.total_sell_quoted += it.sell.cummulativeQuoteQty;
    }

    return res;
}

/*************************************************************************************************/

void make_trades_report(
     std::ostream &os
    ,rest::api &api
    ,const rest::account_info_t &accinfo
    ,const rest::exchange_info_t &exinfo
    ,const std::vector<std::string> &pairs
    ,const std::function<void(const rest::order_info_t &)> &tick
    ,const std::uint64_t start_time
    ,const std::string &start_time_str)
{
    // TODO: comment out
    //binapi::io_state_saver saver(std::cout, 8, fmtflags);

    const auto mtime_from = start_time > 0
        ? start_time
        : !start_time_str.empty()
            ? mstime_from_str(start_time_str)
            : 0
    ;

    auto prices = api.prices();
    assert(prices);

    std::map<std::string, pair_trades> buy_sell_pairs;
    auto mpairs = get_pairs_for_pairs(accinfo, exinfo, pairs);
    std::sort(mpairs.begin(), mpairs.end());
    for ( const auto &pair: mpairs ) {
        auto r_trades = api.my_trades(pair, mtime_from, 0, 0, 1000);
        assert(r_trades);

        if ( r_trades.v.trades.empty() ) { continue; }

        trade_info_container_t mytrades = std::move(r_trades.v.trades);

        // сортирую по времени.
        std::sort(mytrades.begin(), mytrades.end(), [](const auto &l ,const auto &r){ return l.time < r.time; });

        constexpr auto flags = dtf::flags::dd_mm_yyyy|dtf::flags::sep1|dtf::flags::secs;
        std::cout
        << (boost::format("%-10s: ") % pair)
        << "first=" << dtf::timestamp_to_dt_str(mytrades.front().time*1000000ull, flags)
        << ", last=" << dtf::timestamp_to_dt_str(mytrades.back().time*1000000ull, flags) << std::endl;

        // оставляем только SELL ордера
        remove_all_buy_trades(mytrades);

        // получаем пары(BUY-SELL)
        auto full_cycle_trades = get_full_cycle_trades(api, mytrades, tick);
        if ( full_cycle_trades.empty() ) { continue; }

        // считаем сколько купили продали на этой паре
        auto trades_info = calc_pair_trades_info(full_cycle_trades);
        buy_sell_pairs[pair] = std::move(trades_info);
    }

    struct by_pair_profit {
        double_type total_buy;
        double_type total_sell;
        double_type profit;
        std::size_t cycles;
    };

    static const auto summ_cycles = [](const auto &cont) {
        std::size_t total_cycles{};
        for ( const auto &it: cont ) {
            total_cycles += it.second.cycles;
        }

        return total_cycles;
    };

    std::map<std::string, by_pair_profit> total_profit;
    for ( const auto &it: buy_sell_pairs ) {
        // игнорируем те, что по нулям
        if ( it.second.total_buy_base == 0 ) { continue; }

        const auto &syminfo = exinfo.get_by_symbol(it.first);

        // добавляем к магазину.
        // т.е. если мы торговали в USDT магазине несколькими базовыми криптами -
        // тогда профит у нас все равно в USDT, - суммируем его к USDT.
        auto &market = total_profit[syminfo.quoteAsset];
        market.total_buy += it.second.total_buy_quoted;
        market.total_sell += it.second.total_sell_quoted;
        // считаем профит, от суммы продажи вычитаем сумму покупки
        auto diff = it.second.total_sell_quoted - it.second.total_buy_quoted;
        market.profit += diff;
        market.cycles += it.second.cycles;

        //std::cout << syminfo.quoteAsset << " " << diff << std::endl;
        os << (boost::format("%-10s: %11s") % it.first % diff.str(8, iofmt)) << " " << syminfo.quoteAsset;

        // следующий код нужен для того, чтоб в случае если у нас котируемая
        // валюта не является USDT - отобразить профит в пересчете к USDT.
        std::string pair = syminfo.quoteAsset;
        pair += "USDT";
        bool is_valid_pair = syminfo.quoteAsset == "USDT" ? false : exinfo.is_valid_symbol(pair);
        if ( is_valid_pair ) {
            const auto &price = prices.v.get_by_symbol(pair);
            auto total = diff * price.price;
            os << " (" << total << " USDT)";
        }

        os
        << " (" << it.second.cycles << " trades)"
        << std::endl;
    }

    if ( !total_profit.empty() ) {
        os
        << std::endl
        << "PROFIT RESUME:" << std::endl;

        for ( const auto &it: total_profit ) {
            os
            << "  " << it.second.profit << " " << it.first
            << " (BUY: " << it.second.total_buy << ", SELL: " << it.second.total_sell << ", TRADES: " << summ_cycles(total_profit) << ")";

            // следующий код нужен для того, чтоб в случае если у нас котируемая
            // валюта не является USDT - отобразить профит в пересчете к USDT.
            std::string pair = it.first;
            pair += "USDT";
            auto is_valid_pair = it.first == "USDT" ? false : exinfo.is_valid_symbol(pair);
            if ( is_valid_pair ) {
                const auto &price = prices.v.get_by_symbol(pair);
                auto total = it.second.profit * price.price;
                os << " (" << total << " USDT)";
            }
            os << std::endl;
        }
    }
}

void make_trades_report_for_last_day(
     std::ostream &os
    ,rest::api &api
    ,const rest::account_info_t &accinfo
    ,const rest::exchange_info_t &exinfo
    ,const std::vector<std::string> &pairs
    ,const std::function<void(const rest::order_info_t &)> &tick)
{
    auto now = static_cast<std::uint64_t>(std::time(nullptr));
    auto mtime = now - 24*60*60;
    mtime *= 1000;

    return make_trades_report(os, api, accinfo, exinfo, pairs, tick, mtime);
}

/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/

rest::orders_info_t get_open_orders(
     rest::api &api
    ,const rest::exchange_info_t &exinfo
    ,const std::vector<std::string> &pairs
    ,const std::function<void(const std::string &)> &tick
    ,const char *side
    ,const std::uint64_t start_time
    ,const std::string &start_time_str)
{
    const auto mstart_time = start_time > 0
        ? start_time
        : !start_time_str.empty()
            ? mstime_from_str(start_time_str)
            : 0
    ;

    binapi::rest::orders_info_t orders;
    if ( pairs.empty() ) {
        auto req = api.open_orders(nullptr);
        assert(req);
        orders = std::move(req.v);
    } else {
        for ( const auto &it: pairs ) {
            auto tmp = binapi::process_pairs(it, "", exinfo);
            for ( const auto &pit: tmp ) {
                if ( tick ) { tick(pit); }

                auto req = api.open_orders(pit);
                assert(req);
                if ( req.v.orders.empty() ) { continue; }

                for ( auto &oit: req.v.orders ) {
                    auto &vec = orders.orders[oit.first];
                    vec.insert(vec.end(), oit.second.begin(), oit.second.end());
                }
            }
        }
    }

    if ( side ) {
        binapi::rest::orders_info_t tmp;
        for ( auto &pit: orders.orders ) {
            for ( auto &it: pit.second ) {
                if ( it.side == side ) {
                    tmp.orders[it.symbol].push_back(std::move(it));
                }
            }
        }

        orders.orders = std::move(tmp.orders);
    }

    if ( mstart_time ) {
        binapi::rest::orders_info_t tmp;
        for ( auto &pit: orders.orders ) {
            for ( auto &it: pit.second ) {
                if ( it.time >= mstart_time ) {
                    tmp.orders[it.symbol].push_back(std::move(it));
                }
            }
        }

        orders.orders = std::move(tmp.orders);
    }

    return orders;
}

/*************************************************************************************************/

void make_open_orders_resume(
     std::ostream &os
    ,binapi::rest::orders_info_t &orders
    ,const binapi::rest::exchange_info_t &exinfo)
{
    binapi::double_type buy_total, sell_total;
    for ( const auto &pit: orders.orders ) {
        os << pit.first << ":" << std::endl;
        const auto &sym = exinfo.get_by_symbol(pit.first);

        std::size_t buy_count{}, sell_count{};
        binapi::double_type buy_for_pair, sell_for_pair;
        binapi::double_type buy_amount, sell_amount;
        for ( const auto &oit: pit.second ) {
            if ( oit.side == "BUY" ) {
                ++buy_count;
                buy_for_pair += (oit.price * oit.origQty);
                buy_amount += oit.origQty;
            } else if ( oit.side == "SELL" ) {
                ++sell_count;
                sell_for_pair += (oit.price * oit.origQty);
                sell_amount += oit.origQty;
            } else {
                assert(!"unreachable!");
            }
        }

        buy_total += buy_for_pair;
        sell_total += sell_for_pair;

        if ( buy_count ) {
            os << "  B: " << buy_count << " orders, total: " << buy_amount << " " << sym.baseAsset << " = " << buy_for_pair << " " << sym.quoteAsset << std::endl;
        }
        if ( sell_count ) {
            os << "  S: " << sell_count << " orders, total: " << sell_amount << " " << sym.baseAsset << " = " << sell_for_pair << " " << sym.quoteAsset << std::endl;
        }
        os << std::endl;
    }

    if ( buy_total ) {
        os << "BUY  TOTAL: " << buy_total << std::endl;
    }
    if ( sell_total ) {
        os << "SELL TOTAL: " << sell_total << std::endl;
    }
}

void make_open_orders_detailed(
     std::ostream &os
    ,binapi::rest::orders_info_t &orders
    ,const binapi::rest::exchange_info_t &exinfo)
{
    (void)exinfo;

    static const auto print_buy_or_sell_orders_info = [](std::ostream &os, bool buy_or_sell, const std::vector<rest::order_info_t> &orders)
    {
        for ( const auto &oit: orders ) {
            if ( buy_or_sell ) {
                if ( oit.side[0] == 'B' ) {
                    os << boost::format(" B: %10u|%10g|%8g|%10g|") % oit.orderId % oit.price % oit.origQty % (oit.price * oit.origQty) << std::endl;
                }
            } else {
                if ( oit.side[0] == 'S' ) {
                    os << boost::format(" S: %10u|%10g|%8g|%10g|") % oit.orderId % oit.price % oit.origQty % (oit.price * oit.origQty) << std::endl;
                }
            }
        }
    };

    for ( auto &pit: orders.orders ) {
        os << pit.first << ":" << std::endl;
        std::sort(
             pit.second.begin()
            ,pit.second.end()
            ,[]
             (const rest::order_info_t &l, const rest::order_info_t &r)
             { return l.side < r.side; }
        );
        os << "         ID   |   PRICE  | AMOUNT |   TOTAL  |" << std::endl;

        print_buy_or_sell_orders_info(os, true, pit.second);
        print_buy_or_sell_orders_info(os, false, pit.second);

        os << std::endl;
    }
}

void make_open_orders_report(
     std::ostream &os
    ,rest::api &api
    ,const rest::exchange_info_t &exinfo
    ,const std::vector<std::string> &pairs
    ,const std::function<void(const std::string &)> &tick
    ,const std::uint64_t start_time
    ,const std::string &start_time_str)
{
    rest::orders_info_t orders = get_open_orders(api, exinfo, pairs, tick, nullptr, start_time, start_time_str);
    if ( !orders.orders.empty() ) {
        make_open_orders_resume(os, orders, exinfo);
        os << std::endl;
        make_open_orders_detailed(os, orders, exinfo);
    }
}

/*************************************************************************************************/

void make_balance_report(
    std::ostream &os
    ,rest::api &api
    ,const rest::account_info_t &accinfo
    ,const rest::exchange_info_t &exinfo)
{
    (void)api;
    (void)exinfo;

    for ( const auto &it: accinfo.balances ) {
        if ( it.second.free ) {
            os << boost::format("%-10s: ") % it.first;
            os << it.second.free << std::endl;
        }
    }
}

/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/

void show_exchanger_price_for_orders(
     std::ostream &os
    ,rest::api &api
    ,const rest::exchange_info_t &exinfo
    ,const std::vector<std::string> &pairs
    ,const std::function<void(const std::string &)> &tick)
{
    rest::orders_info_t orders = get_open_orders(api, exinfo, pairs, tick, "SELL");
    auto r_prices = api.prices();
    assert(r_prices);
    rest::prices_t prices = std::move(r_prices.v);

    struct out_item {
        std::string pair;
        double_type exchanger_price;
        std::vector<double_type> order_price;
    };
    std::vector<out_item> out_items;

    for ( auto &it: orders.orders ) {
        std::sort(
             it.second.begin()
            ,it.second.end()
            ,[](const rest::order_info_t &l, const rest::order_info_t &r)
             { return l.price < r.price; }
        );
        out_item item;
        item.exchanger_price = prices.get_by_symbol(it.first).price;
        item.pair = std::move(it.first);
        for ( auto &pit: it.second ) {
            item.order_price.push_back(std::move(pit.price));
        }
        out_items.push_back(std::move(item));
    }

    // sort by minimum price difference between exchanger and min order price
    std::sort(
         out_items.begin()
        ,out_items.end()
        ,[](const out_item &l, const out_item &r) {
             const auto lp = percents_diff(l.exchanger_price, l.order_price.front());
             const auto rp = percents_diff(r.exchanger_price, r.order_price.front());
             return lp < rp;
         }
    );

    for ( const auto &it: out_items ) {
        const auto price_prec = binapi::num_fractions_from_double_type(exinfo.get_by_symbol(it.pair).get_filter_price().tickSize);
        os
        << it.pair << ": " << it.exchanger_price.str(price_prec, iofmt) << ", "
        << percents_diff(prices.get_by_symbol(it.pair).price, it.order_price.front()).str(2, iofmt) << "% : ";
        for ( auto oit = it.order_price.begin(); oit != it.order_price.end(); ++oit ) {
            os << oit->str(price_prec, iofmt);
            if ( std::next(oit) != it.order_price.end() ) {
                os << ", ";
            }
        }
        os << std::endl;
    }
}

/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/

void calc_loss_for_orders(
    std::ostream &os
    ,rest::api &api
    ,const rest::exchange_info_t &exinfo
    ,const std::vector<std::string> &pairs
    ,const std::function<void(const std::string &)> &tick)
{
    rest::orders_info_t orders = get_open_orders(api, exinfo, pairs, tick, "SELL");
    auto r_prices = api.prices();
    assert(r_prices);
    rest::prices_t prices = std::move(r_prices.v);

    struct out_item {
        std::string pair;
        double_type loss;
    };
    std::vector<out_item> out_items;

    for ( const auto &it: orders.orders ) {
        // skip the pair which contains only one order and it not created by bot
        if ( it.second.size() == 1 && !is_my_orderid(it.second.front().clientOrderId) ) {
            continue;
        }

        const auto &price = prices.get_by_symbol(it.first).price;
        double_type exp{}, real{};
        for ( const auto &pit: it.second ) {
            // skip order because it not created by bot
            if ( !is_my_orderid(pit.clientOrderId) ) {
                continue;
            }

            exp += (pit.price * pit.origQty);
            real += (price * pit.origQty);
        }

        out_item item;
        item.loss = real - exp;
        item.pair = it.first;
        out_items.push_back(std::move(item));
    }

    // sort by loss
    std::sort(
         out_items.begin()
        ,out_items.end()
        ,[](const out_item &l, const out_item &r)
         { return l.loss > r.loss; }
    );

    double_type total_loss{};
    for ( const auto &it: out_items ) {
        total_loss += it.loss;
        const auto &sym = exinfo.get_by_symbol(it.pair);
        const auto price_prec = binapi::num_fractions_from_double_type(exinfo.get_by_symbol(it.pair).get_filter_price().tickSize);
        os
        << it.pair << ": " << it.loss.str(price_prec, iofmt) << " " << sym.quoteAsset << std::endl;
    }
    os
    << "   TOTAL LOSS: " << total_loss.str(2, iofmt) << std::endl;
}

/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/

} // ns binapi
