
#include "enums.hpp"
#include "fnv1a.hpp"

#include <cstring>
#include <cassert>

namespace binapi {

/*************************************************************************************************/

e_side e_side_from_string(const char *str) {
    const auto hash = fnv1a(str);
    switch ( hash ) {
        case fnv1a("BUY"): return e_side::buy;
        case fnv1a("SELL"): return e_side::sell;
    }

    assert(!"unreachable");
}

/*************************************************************************************************/

e_type e_type_from_string(const char *str) {
    const auto hash = fnv1a(str);
    switch ( hash ) {
        case fnv1a("LIMIT"): return e_type::limit;
        case fnv1a("MARKET"): return e_type::market;
        case fnv1a("STOP_LOSS"): return e_type::stop_loss;
        case fnv1a("STOP_LOSS_LIMIT"): return e_type::stop_loss_limit;
        case fnv1a("TAKE_PROFIT"): return e_type::take_profit;
        case fnv1a("TAKE_PROFIT_LIMIT"): return e_type::take_profit_limit;
        case fnv1a("LIMIT_MAKER"): return e_type::limit_maker;
    }

    assert(!"unreachable");
}

/*************************************************************************************************/

e_time e_time_from_string(const char *str) {
    const auto hash = fnv1a(str);
    switch ( hash ) {
        case fnv1a("GTC"): return e_time::GTC;
        case fnv1a("IOC"): return e_time::IOC;
        case fnv1a("FOK"): return e_time::FOK;
    }

    assert(!"unreachable");
}

/*************************************************************************************************/

} // ns binapi
