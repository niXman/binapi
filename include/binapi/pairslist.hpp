
// ----------------------------------------------------------------------------
//                              Apache License
//                        Version 2.0, January 2004
//                     http://www.apache.org/licenses/
//
// This file is part of binapi(https://github.com/niXman/binapi) project.
//
// Copyright (c) 2019-2021 niXman (github dot nixman dog pm.me). All rights reserved.
// ----------------------------------------------------------------------------

#ifndef __binapi__pairslist_hpp
#define __binapi__pairslist_hpp

#include <string>
#include <set>

namespace binapi {
namespace rest {

struct exchange_info_t;

} // ns rest

/*************************************************************************************************/

// create the exchange available pairs list according to black and white lists
std::set<std::string> process_pairs(
     const std::string &whitelist
    ,const std::string &blacklist
    ,const binapi::rest::exchange_info_t &exinfo
);

bool pair_in_pairs(const std::set<std::string> &pairs, const std::string &pair);

void test_blackwhite_list();

/*************************************************************************************************/

} // ns binapi

#endif // __binapi__pairslist_hpp
