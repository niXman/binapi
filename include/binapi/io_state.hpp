
#ifndef __binapi__io_state_hpp
#define __binapi__io_state_hpp

#include "iofmt.hpp"

#include <iosfwd>

namespace binapi {

/*************************************************************************************************/

struct io_state_saver {
    io_state_saver(std::ostream &os, std::size_t prec, std::ios_base::fmtflags flags = iofmt);
    ~io_state_saver();

private:
    struct impl;
    impl *pimpl;
};

/*************************************************************************************************/

} // ns binapi

#endif // __binapi__io_state_hpp
