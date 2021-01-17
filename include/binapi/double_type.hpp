
// ----------------------------------------------------------------------------
//                              Apache License
//                        Version 2.0, January 2004
//                     http://www.apache.org/licenses/
//
// This file is part of binapi(https://github.com/niXman/binapi) project.
//
// Copyright (c) 2019-2021 niXman (github dot nixman dog pm.me). All rights reserved.
// ----------------------------------------------------------------------------

#ifndef __binapi__double_type_hpp
#define __binapi__double_type_hpp

#include <boost/multiprecision/cpp_dec_float.hpp>

/*************************************************************************************************/

namespace binapi {

using double_type = boost::multiprecision::number<
     boost::multiprecision::cpp_dec_float<8>
    ,boost::multiprecision::et_off
>;

} // ns binapi

/*************************************************************************************************/

#endif // __binapi__double_type_hpp
