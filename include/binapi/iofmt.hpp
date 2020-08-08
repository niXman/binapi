
#ifndef __binapi__iofmt_hpp
#define __binapi__iofmt_hpp

#include <iomanip>

namespace binapi {

constexpr std::ios_base::fmtflags iofmt = std::ios_base::fixed|std::ios_base::showpoint|std::ios_base::showbase;

} // ns binapi

#endif // __binapi__iofmt_hpp
