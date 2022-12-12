
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

#ifndef __bg_api__pairslist_hpp
#define __bg_api__pairslist_hpp

#include <string>
#include <set>

namespace bg_api {
namespace rest {

struct exchange_info_t;

} // ns rest

/*************************************************************************************************/

// create the exchange available pairs list according to black and white lists
std::set<std::string> process_pairs(
     const std::string &whitelist
    ,const std::string &blacklist
    ,const bg_api::rest::exchange_info_t &exinfo
);

bool pair_in_pairs(const std::set<std::string> &pairs, const std::string &pair);

void test_blackwhite_list();

/*************************************************************************************************/

} // ns bg_api

#endif // __bg_api__pairslist_hpp
