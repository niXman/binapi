
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

#include "bg_api/websocket.hpp"
#include "bg_api/api.hpp"
#include "bg_api/flatjson.hpp"

#include <boost/asio/io_context.hpp>
#include <boost/asio/steady_timer.hpp>

#include <iostream>
#include <fstream>
#include <bg_api/errors.hpp>

/*************************************************************************************************/

std::string read_file(const char *fname) {
    std::ifstream file(fname);
    assert(file);

    std::string res{
         (std::istreambuf_iterator<char>(file))
        ,(std::istreambuf_iterator<char>())
    };

    return res;
}

/*************************************************************************************************/

#define PRINT_IF_ERROR(res) \
    if ( !static_cast<bool>(res) ) { \
        std::cout << __FILE__ << "(" << __LINE__ << "): msg=" << res.errmsg << std::endl; \
    }

/*************************************************************************************************/

#define BREAK_IF_ERROR(res) \
    PRINT_IF_ERROR(res); \
    if ( !res ) { \
        return EXIT_FAILURE; \
    }

/*************************************************************************************************/

int main(int argc, char **argv) {
    assert(argc == 3);
    const std::string pk = argv[1];
    const std::string passphrase = argv[2];
    const std::string sk = argv[3];

    std::cout.precision(15);

    boost::asio::io_context ioctx;
    bg_api::ws::websockets wsp(ioctx, "ws.bitget.com", "443", pk, passphrase, sk);

    bg_api::rest::api api(
        ioctx,
        "api.bitget.com",
        "443",
        pk,
        passphrase,
        sk,
        30
    );

    
    ioctx.run();

    return 0;
}

/*************************************************************************************************/
