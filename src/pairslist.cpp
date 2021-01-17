
// ----------------------------------------------------------------------------
//                              Apache License
//                        Version 2.0, January 2004
//                     http://www.apache.org/licenses/
//
// This file is part of binapi(https://github.com/niXman/binapi) project.
//
// Copyright (c) 2019-2021 niXman (github dot nixman dog pm.me). All rights reserved.
// ----------------------------------------------------------------------------

#include <binapi/pairslist.hpp>
#include <binapi/tools.hpp>
#include <binapi/types.hpp>

#include <boost/algorithm/string/predicate.hpp>

#include <cassert>

namespace binapi {

/*************************************************************************************************/

std::set<std::string> process_pairs(
     const std::string &whitelist
    ,const std::string &blacklist
    ,const binapi::rest::exchange_info_t &exinfo
) {
    std::set<std::string> res;

    assert(!whitelist.empty());

    if ( !whitelist.empty() ) {
        auto p = binapi::split_string(whitelist, ",");
        if ( p.size() == 1 && p.front() == "*" ) {
            for ( const auto &sit: exinfo.symbols ) {
                res.insert(sit.first);
            }
        } else {
            for ( auto &pit: p ) {
                if ( pit.front() == '*' ) { // *USDT
                    auto quoted = pit.substr(1);
                    for ( const auto &sit: exinfo.symbols ) {
                        if ( sit.second.quoteAsset == quoted ) {
                            res.insert(sit.first);
                        }
                    }
                } else if ( pit.back() == '*' ) { // BTC*
                    auto base = pit.substr(0, pit.length() - 1);
                    for ( const auto &sit: exinfo.symbols ) {
                        if ( sit.second.baseAsset == base ) {
                            res.insert(sit.first);
                        }
                    }
                } else {
                    assert(exinfo.symbols.count(pit));

                    res.insert(std::move(pit));
                }
            }
        }
    }

    // white list was applied, apply black list.
    if ( !res.empty() && !blacklist.empty() ) {
        auto p = binapi::split_string(blacklist, ",");
        if ( p.size() == 1 && p.front() == "*" ) {
            res.clear();
        } else {
            for ( auto &pit: p ) {
                if ( pit.front() == '*' ) { // *USDT
                    auto quoted = pit.substr(1);
                    for ( auto sit = res.begin(); sit != res.end(); ) {
                        if ( boost::algorithm::ends_with(*sit, quoted) ) {
                            sit = res.erase(sit);
                        } else {
                            ++sit;
                        }
                    }
                } else if ( pit.back() == '*' ) { // BTC*
                    auto base = pit.substr(0, pit.length() - 1);
                    for ( auto sit = res.begin(); sit != res.end();  ) {
                        if ( boost::algorithm::starts_with(*sit, base) ) {
                            sit = res.erase(sit);
                        } else {
                            ++sit;
                        }
                    }
                } else {
                    assert(exinfo.symbols.count(pit));

                    for ( auto sit = res.begin(); sit != res.end(); ) {
                        if ( pit == *sit ) {
                            sit = res.erase(sit);
                        } else {
                            ++sit;
                        }
                    }
                }
            }
        }

        // white list was not used - get pairs from
        // exchange and filter it using black list.
    } else if ( !blacklist.empty() ) {
        auto p = binapi::split_string(blacklist, ",");
        for ( auto &pit: p ) {
            if ( pit.front() == '*' ) { // *USDT
                auto quoted = pit.substr(1);
                for ( const auto &sit: exinfo.symbols ) {
                    if ( !boost::algorithm::ends_with(sit.first, quoted)) {
                        res.insert(sit.first);
                    }
                }
            } else if ( pit.back() == '*' ) { // BTC*
                auto base = pit.substr(0, pit.length() - 1);
                for ( const auto &sit: exinfo.symbols ) {
                    if ( !boost::algorithm::starts_with(sit.first, base)) {
                        res.insert(sit.first);
                    }
                }
            } else {
                for ( const auto &sit: exinfo.symbols ) {
                    if ( pit == sit.first ) continue;
                    res.insert(sit.first);
                }
            }
        }
    }

    return res;
}

/*************************************************************************************************/

bool pair_in_pairs(const std::set<std::string> &pairs, const std::string &pair) {
    return pairs.find(pair) != pairs.end();
}

/*************************************************************************************************/

void test_blackwhite_list() {
    struct pair_info {
        const char *pair;
        const char *base;
        const char *quote;
    };

#define __MAKE_PAIR(base, quoted) \
    {base quoted, base, quoted}

    static const pair_info pairs[] = {
         __MAKE_PAIR("ADA", "BNB")
        ,__MAKE_PAIR("ADA", "BTC")
        ,__MAKE_PAIR("BNB", "ADA")
        ,__MAKE_PAIR("BTC", "ADA")
    };

    binapi::rest::exchange_info_t exinfo{};
    for ( const auto &it: pairs ) {
        binapi::rest::exchange_info_t::symbol_t item{};
        item.symbol = it.pair;
        item.baseAsset = it.base;
        item.quoteAsset = it.quote;

        exinfo.symbols.emplace(it.pair, std::move(item));
    }

    auto list0 = process_pairs("*", "", exinfo);
    assert(
        list0.size() == 4 &&
        pair_in_pairs(list0, "ADABNB") &&
        pair_in_pairs(list0, "ADABTC") &&
        pair_in_pairs(list0, "BNBADA") &&
        pair_in_pairs(list0, "BTCADA")
    );

    auto list1 = process_pairs("ADA*", "", exinfo);
    assert(
        list1.size() == 2 &&
        pair_in_pairs(list1, "ADABNB") &&
        pair_in_pairs(list1, "ADABTC") &&
        !pair_in_pairs(list1, "BNBADA") &&
        !pair_in_pairs(list1, "BTCADA")
    );

    auto list2 = process_pairs("*ADA", "", exinfo);
    assert(
        list2.size() == 2 &&
        !pair_in_pairs(list2, "ADABNB") &&
        !pair_in_pairs(list2, "ADABTC") &&
        pair_in_pairs(list2, "BNBADA") &&
        pair_in_pairs(list2, "BTCADA")
    );

    auto list3 = process_pairs("BNBADA", "", exinfo);
    assert(
        list3.size() == 1 &&
        !pair_in_pairs(list3, "ADABNB") &&
        !pair_in_pairs(list3, "ADABTC") &&
        pair_in_pairs(list3, "BNBADA") &&
        !pair_in_pairs(list3, "BTCADA")
    );

    auto list4 = process_pairs("*", "*", exinfo);
    assert(list4.size() == 0);

    auto list5 = process_pairs("ADA*", "ADA*", exinfo);
    assert(list5.size() == 0);

    auto list6 = process_pairs("*ADA", "BNB*", exinfo);
    assert(
        list6.size() == 1 &&
        pair_in_pairs(list6, "BTCADA")
    );

    auto list7 = process_pairs("BNBADA", "BNBADA", exinfo);
    assert(list7.size() == 0);
}

/*************************************************************************************************/

} // ns binapi
