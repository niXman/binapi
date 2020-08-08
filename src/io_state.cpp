
// ----------------------------------------------------------------------------
//                              Apache License
//                        Version 2.0, January 2004
//                     http://www.apache.org/licenses/
//
// This file is part of binapi(https://github.com/niXman/binapi) project.
//
// Copyright (c) 2019-2020 niXman (github dot nixman dog pm.me). All rights reserved.
// ----------------------------------------------------------------------------

#include <binapi/io_state.hpp>

#include <boost/io/ios_state.hpp>

namespace binapi {

/*************************************************************************************************/

struct io_state_saver::impl {
    impl(std::ostream &os, std::size_t prec, std::ios_base::fmtflags flags)
        :m_saver{os}
    {
        os.precision(prec);
        os.flags(flags);
    }

    ~impl()
    {}

    boost::io::ios_all_saver m_saver;
};

/*************************************************************************************************/

io_state_saver::io_state_saver(std::ostream &os, std::size_t prec, std::ios_base::fmtflags flags)
    :pimpl{new impl(os, prec, flags)}
{}

io_state_saver::~io_state_saver()
{ delete pimpl; }

/*************************************************************************************************/

} // ns binapi
