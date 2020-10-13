
// ----------------------------------------------------------------------------
//                              Apache License
//                        Version 2.0, January 2004
//                     http://www.apache.org/licenses/
//
// This file is part of binapi(https://github.com/niXman/binapi) project.
//
// Copyright (c) 2019-2020 niXman (github dot nixman dog pm.me). All rights reserved.
// ----------------------------------------------------------------------------

#ifndef __binapi__invoker_hpp
#define __binapi__invoker_hpp

#include <memory>
#include <cstdint>
#include <cstring>
#include <cstdio>

#include "message.hpp"

namespace binapi {
namespace detail {

/*************************************************************************************************/

struct invoker_base {
    virtual bool invoke(const char *fl, int ec, std::string errmsg, const char *ptr, std::size_t size) = 0;
};

using invoker_ptr = std::shared_ptr<invoker_base>;

/*************************************************************************************************/

template<typename R, typename T, typename F>
struct invoker: invoker_base {
    invoker(F f)
        :m_cb{std::move(f)}
    {}
    virtual ~invoker() = default;

    bool invoke(const char *fl, int ec, std::string errmsg, const char *ptr, std::size_t size) override {
        try {
            if ( !size || ec ) {
                T arg{};
                return m_cb(fl, ec, std::move(errmsg), std::move(arg));
            } else {
                if ( std::strstr(ptr, "\"code\":") && std::strstr(ptr, "\"msg\":") ) {
                    T arg{};
                    return m_cb(__MAKE_FILELINE, -1, ptr, std::move(arg));
                } else {
                    T arg = T::parse(ptr, size);
                    return m_cb(__MAKE_FILELINE, 0, std::move(errmsg), std::move(arg));
                }
            }
        } catch (const std::exception &ex) {
            std::fprintf(stderr, "%s(%d): ex=%s\n", __FILE__, __LINE__, ex.what());
            std::fprintf(stderr, "size=%u, ptr=%s\n", (unsigned)size, ptr);
            std::fflush(stderr);
        }

        return true;
    }

    F m_cb;
};

/*************************************************************************************************/

} // ns detail
} // ns binapi

#endif // __binapi__invoker_hpp
