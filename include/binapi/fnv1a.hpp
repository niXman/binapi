
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
