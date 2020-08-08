
#ifndef __binapi__enums_hpp
#define __binapi__enums_hpp

#include <cstdint>

namespace binapi {

/*************************************************************************************************/

enum class e_side: std::size_t {
    buy
   ,sell
};

e_side e_side_from_string(const char *str);

/*************************************************************************************************/

enum class e_type: std::size_t {
     limit
    ,market
    ,stop_loss
    ,stop_loss_limit
    ,take_profit
    ,take_profit_limit
    ,limit_maker
};

e_type e_type_from_string(const char *str);

/*************************************************************************************************/

enum class e_time: std::size_t {
     GTC
    ,IOC
    ,FOK
};

e_time e_time_from_string(const char *str);

/*************************************************************************************************/

} // ns binapi

#endif // __binapi__enums_hpp
