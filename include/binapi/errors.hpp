
// ----------------------------------------------------------------------------
//                              Apache License
//                        Version 2.0, January 2004
//                     http://www.apache.org/licenses/
//
// This file is part of binapi(https://github.com/niXman/binapi) project.
//
// Copyright (c) 2019-2021 niXman (github dot nixman dog pm.me). All rights reserved.
// ----------------------------------------------------------------------------

#ifndef __binapi__errors_hpp
#define __binapi__errors_hpp

#include <string>

/*************************************************************************************************/

namespace binapi {


struct error_reply {
    /* from 'ccxt' project:
    '-1000': ExchangeNotAvailable, // {"code":-1000,"msg":"An unknown error occured while processing the request."}
    '-1013': InvalidOrder, // createOrder -> 'invalid quantity'/'invalid price'/MIN_NOTIONAL
    '-1021': InvalidNonce, // 'your time is ahead of server'
    '-1022': AuthenticationError, // {"code":-1022,"msg":"Signature for this request is not valid."}
    '-1100': InvalidOrder, // createOrder(symbol, 1, asdf) -> 'Illegal characters found in parameter 'price'
    '-1104': ExchangeError, // Not all sent parameters were read, read 8 parameters but was sent 9
    '-1128': ExchangeError, // {"code":-1128,"msg":"Combination of optional parameters invalid."}
    '-2010': ExchangeError, // generic error code for createOrder -> 'Account has insufficient balance for requested action.', {"code":-2010,"msg":"Rest API trading is not enabled."}, etc...
    '-2011': OrderNotFound, // cancelOrder(1, 'BTC/USDT') -> 'UNKNOWN_ORDER'
    '-2013': OrderNotFound, // fetchOrder (1, 'BTC/USDT') -> 'Order does not exist'
    '-2014': AuthenticationError, // { "code":-2014, "msg": "API-key format invalid." }
    '-2015': AuthenticationError, // "Invalid API-key, IP, or permissions for action."
    */

    int code;
    std::string message;
};

error_reply parse_error(const std::string &msg);

} // ns binapi

/*************************************************************************************************/

#endif // __binapi__errors_hpp
