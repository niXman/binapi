
// MIT License
//
// Copyright (c) 2019 niXman (i dotty nixman doggy gmail dotty com)
// All rights reserved.
//
// This file is part of DTF(https://github.com/niXman/dtf) project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef __dtf__dtf_hpp
#define __dtf__dtf_hpp

#include <string>

#include <cstdint>

namespace dtf {

/*************************************************************************************************/

// always in nanoseconds resolution
std::uint64_t timestamp(int offset = 0 /*in hours*/);

struct flags {
    enum: std::size_t {
         yyyy_mm_dd = 1u << 0u
        ,dd_mm_yyyy = 1u << 1u
        ,sep1       = 1u << 2u // 2018-12-11 13:58:56
        ,sep2       = 1u << 3u // 2018.12.11-13.58.59
        ,sep3       = 1u << 4u // 2018.12.11-13:58:59
        ,secs       = 1u << 5u // seconds resolution
        ,msecs      = 1u << 6u // milliseconds resolution
        ,usecs      = 1u << 7u // microseconds resolution
        ,nsecs      = 1u << 8u // nanoseconds resolution
    };
};

enum { bufsize = 32 };

// format as number in c-string representation
std::size_t timestamp_to_chars(
     char *buf
    ,std::uint64_t ts
    ,std::size_t f = flags::msecs
);

std::string timestamp_to_str(
     std::uint64_t ts
    ,std::size_t f = flags::msecs
);

std::string timestamp_str(std::size_t f = flags::msecs, int offset = 0);

// formats as date-time string
// returns the num of bytes placed
std::size_t timestamp_to_dt_chars(
     char *ptr // dst buf with at least 'bufsize' bytes
    ,std::uint64_t ts
    ,std::size_t f = flags::yyyy_mm_dd|flags::sep1|flags::msecs
);

std::string timestamp_to_dt_str(
     std::uint64_t ts
    ,std::size_t f = flags::yyyy_mm_dd|flags::sep1|flags::msecs
);

std::string timestamp_dt_str(
     std::size_t f = flags::yyyy_mm_dd|flags::sep1|flags::msecs
    ,int offset = 0
);

/*************************************************************************************************/

// gets the respective flags for date-time string
std::size_t dt_str_flags(const char *buf, std::size_t n);

/*************************************************************************************************/

} // ns dtf

#ifdef DTF_HEADER_ONLY
#   include "dtf.cpp"
#endif // DTF_HEADER_ONLY

#endif // __dtf__dtf_hpp
