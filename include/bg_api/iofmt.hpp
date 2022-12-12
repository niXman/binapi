
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

#ifndef __bg_api__iofmt_hpp
#define __bg_api__iofmt_hpp

#include <iomanip>

namespace bg_api {

constexpr std::ios_base::fmtflags iofmt = std::ios_base::fixed|std::ios_base::showpoint|std::ios_base::showbase;

} // ns bg_api

#endif // __bg_api__iofmt_hpp
