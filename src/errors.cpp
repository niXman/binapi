
// ----------------------------------------------------------------------------
//                              Apache License
//                        Version 2.0, January 2004
//                     http://www.apache.org/licenses/
//
// This file is part of binapi(https://github.com/niXman/binapi) project.
//
// Copyright (c) 2019-2021 niXman (github dot nixman dog pm.me). All rights reserved.
// ----------------------------------------------------------------------------

#include "errors.hpp"
#include "json.hpp"

#include <cassert>
#include <cstring>

namespace binapi {

/*************************************************************************************************/

error_reply parse_error(const std::string &msg) {
    assert(std::strstr(msg.c_str(), "\"code\":") && std::strstr(msg.c_str(), "\"msg\":"));

    nlohmann::json j = nlohmann::json::parse(msg.begin(), msg.end());

    error_reply res{};
    res.code = j.at("code").get<int>();
    res.message = j.at("msg").get<std::string>();

    return res;
}

/*************************************************************************************************/

} // ns binapi
