
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

#ifndef __bg_api__tools_hpp
#define __bg_api__tools_hpp

#include "double_type.hpp"

#include <string>
#include <vector>

namespace bg_api {

/*************************************************************************************************/

std::size_t num_fractions_from_double_type(const double_type &v);

double_type adjust_to_step(const double_type &v, const double_type &s, bool increase = false);

std::vector<std::string> split_string(const std::string &str, const char *sep);
std::string join_string(const std::vector<std::string> &vec, const char *sep);

/*************************************************************************************************/

double_type percents_diff(const double_type &a, const double_type &b);
double_type percents_add(const double_type &v, const double_type &p);
double_type percents_sub(const double_type &v, const double_type &p);
double_type percents_val_by_percent(const double_type &v, const double_type &p);

/*************************************************************************************************/

bool is_my_orderid(const std::string &client_order_id);
bool is_my_orderid(const char *client_order_id);

/*************************************************************************************************/

} // ns bg_api

#endif // __bg_api__tools_hpp
