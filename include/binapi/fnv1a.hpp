
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
#include <type_traits>

namespace binapi {

/*************************************************************************************************/

template<typename CharT>
constexpr std::size_t ct_strlen(const CharT *s) {
    const CharT *b = s;
    for ( ; *s; ++s )
        ;

    return s - b;
}

/*************************************************************************************************/

template<typename CharT>
constexpr std::uint32_t fnv1a(const CharT *s, std::size_t len) {
    std::uint32_t seed = 0x811c9dc5;
    for ( ; len; --len, ++s ) {
        seed = static_cast<std::uint32_t>(
            (seed ^ static_cast<std::uint32_t>(*s)) * static_cast<std::uint64_t>(0x01000193)
        );
    }

    return seed;
}

template<
     typename ConstCharPtr
    ,typename = typename std::enable_if<
        std::is_same<ConstCharPtr, const char*>::value
    >::type
>
constexpr std::uint32_t fnv1a(ConstCharPtr s) {
    return fnv1a(s, ct_strlen(s));
}

template<typename CharT, std::size_t N>
constexpr std::uint32_t fnv1a(const CharT (&s)[N]) {
    return fnv1a(s, N-1);
}

/*************************************************************************************************/

} // ns binapi

#endif // __binapi__fnv1a_hpp
