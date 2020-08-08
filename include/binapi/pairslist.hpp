
#ifndef __binapi__pairslist_hpp
#define __binapi__pairslist_hpp

#include <string>
#include <set>

namespace binapi {
namespace rest {

struct exchange_info_t;

} // ns rest

/*************************************************************************************************/

// create the exchange available pairs list according to black and white lists
std::set<std::string> process_pairs(
     const std::string &whitelist
    ,const std::string &blacklist
    ,const binapi::rest::exchange_info_t &exinfo
);

bool pair_in_pairs(const std::set<std::string> &pairs, const std::string &pair);

void test_blackwhite_list();

/*************************************************************************************************/

} // ns binapi

#endif // __binapi__pairslist_hpp
