
// ----------------------------------------------------------------------------
//                              Apache License
//                        Version 2.0, January 2004
//                     http://www.apache.org/licenses/
//
// This file is part of binapi(https://github.com/niXman/binapi) project.
//
// Copyright (c) 2019-2021 niXman (github dot nixman dog pm.me). All rights reserved.
// ----------------------------------------------------------------------------

#ifndef __binapi__iofmt_hpp
#define __binapi__iofmt_hpp

#include <iomanip>

namespace binapi {

constexpr std::ios_base::fmtflags iofmt = std::ios_base::fixed|std::ios_base::showpoint|std::ios_base::showbase;

} // ns binapi

#endif // __binapi__iofmt_hpp
