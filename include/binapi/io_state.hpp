
// ----------------------------------------------------------------------------
//                              Apache License
//                        Version 2.0, January 2004
//                     http://www.apache.org/licenses/
//
// This file is part of binapi(https://github.com/niXman/binapi) project.
//
// Copyright (c) 2019-2020 niXman (github dot nixman dog pm.me). All rights reserved.
// ----------------------------------------------------------------------------

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
