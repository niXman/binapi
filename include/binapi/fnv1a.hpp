
// ----------------------------------------------------------------------------
//                              Apache License
//                        Version 2.0, January 2004
//                     http://www.apache.org/licenses/
//
// This file is part of binapi(https://github.com/niXman/binapi) project.
//
// Copyright (c) 2019-2021 niXman (github dot nixman dog pm.me). All rights reserved.
// ----------------------------------------------------------------------------

#ifndef __binapi__fnv1a_hpp
#define __binapi__fnv1a_hpp

#include <cstdint>

namespace binapi {

/*************************************************************************************************/

template<typename CharT>
constexpr std::uint32_t fnv1a(const CharT *s) {
    std::uint32_t seed = 0x811c9dc5;
    for ( ; *s; ++s ) {
        seed = static_cast<std::uint32_t>(
            (seed ^ static_cast<std::uint32_t>(*s)) * static_cast<std::uint64_t>(0x01000193)
        );
    }

    return seed;
}

/*************************************************************************************************/

} // ns binapi

#endif // __binapi__fnv1a_hpp
