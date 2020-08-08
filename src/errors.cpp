
#include "errors.hpp"
#include "json.hpp"

#include <cassert>
#include <cstring>

namespace binapi {

/*************************************************************************************************/

error_reply parse_error(const std::string &msg) {
    assert(std::strstr(msg.c_str(), "\"code\":") && std::strstr(msg.c_str(), "\"msg\":"));

    nlohmann::json j = nlohmann::json::parse(msg.begin(), msg.end());

    error_reply res{};
    res.code = j.at("code").get<int>();
    res.message = j.at("msg").get<std::string>();

    return res;
}

/*************************************************************************************************/

} // ns binapi
