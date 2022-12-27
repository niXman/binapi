
// ----------------------------------------------------------------------------
//                              Apache License
//                        Version 2.0, January 2004
//                     http://www.apache.org/licenses/
//
// This file is part of bg_api(https://github.com/patrickk33/bg_api) project. A fork of 
// niXman's binapi(https://github.com/niXman/binapi) project.
//
// Copyright (c) 2019-2021 niXman (github dot nixman dog pm.me). All rights reserved.
// ----------------------------------------------------------------------------

#ifndef __bg_api__invoker_hpp
#define __bg_api__invoker_hpp

#include <memory>
#include <cstdint>
#include <cstring>
#include <cstdio>

#include "message.hpp"
#include "errors.hpp"
#include "simdjson.h"

namespace bg_api {
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
                simdjson::padded_string json = simdjson::padded_string(ptr, size);
                simdjson::ondemand::parser parser;
                simdjson::ondemand::document doc;
                auto error = parser.iterate(json).get(doc);

                if (error) {
                    T arg{};
                    std::cerr << error << std::endl;
                    return m_cb(__MAKE_FILELINE, error, "", std::move(arg));
                }

                if (bg_api::rest::is_api_error(doc)) {
                    auto error = bg_api::rest::construct_error(doc);
                    T arg{};
                    return m_cb(__MAKE_FILELINE, error.first, std::move(error.second), std::move(arg));
                } else {
                    T arg = T::construct(doc);
                    return m_cb(__MAKE_FILELINE, 0, std::move(errmsg), std::move(arg));
                }
            }
        } catch (const std::exception &ex) {
            std::fprintf(stderr, "%s: ex=%s\n", __MAKE_FILELINE, ex.what());
            std::fprintf(stderr, "size=%u, ptr=%s\n", (unsigned)size, ptr);
            std::fflush(stderr);
        }

        return true;
    }

    F m_cb;
};

/*************************************************************************************************/

} // ns detail
} // ns bg_api

#endif // __bg_api__invoker_hpp
