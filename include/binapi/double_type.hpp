
#ifndef __binapi__double_type_hpp
#define __binapi__double_type_hpp

#include <boost/multiprecision/cpp_dec_float.hpp>

/*************************************************************************************************/

namespace binapi {

using double_type = boost::multiprecision::number<
     boost::multiprecision::cpp_dec_float<8>
    ,boost::multiprecision::et_off
>;

} // ns binapi

/*************************************************************************************************/

#endif // __binapi__double_type_hpp
