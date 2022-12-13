
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

#include <bg_api/types.hpp>
#include <bg_api/flatjson.hpp>
#include <bg_api/fnv1a.hpp>

#include <type_traits>

//#include <iostream> // TODO: comment out

namespace bg_api {

//------------------------------------------------------------------------------

template<typename T>
typename std::enable_if<std::is_integral<T>::value>::type
__get_json(T &v, const char *member, const flatjson::fjson &j) {
    v = j.at(member).to<T>();
}

template<typename T>
typename std::enable_if<std::is_same<T, std::string>::value>::type
__get_json(T &v, const char *member, const flatjson::fjson &j) {
    const auto &o = j.at(member);
    v = (o.is_null() ? std::string{} : o.to_string());
}

template<typename T>
typename std::enable_if<std::is_same<T, double_type>::value>::type
__get_json(T &v, const char *member, const flatjson::fjson &j) {
    v.assign(j.at(member).to_string());
}

#define __bg_api_GET2(obj, member, json) \
    __get_json(obj.member, #member, json)

#define __bg_api_GET(member) __bg_api_GET2(res, member, json)

//------------------------------------------------------------------------------

namespace rest {


} // ns rest

namespace ws {


} // ns ws

} // ns bg_api
