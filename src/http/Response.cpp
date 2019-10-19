//
// Created by daniel on 17/09/19.
//

#include <cassert>
#include "Response.h"

std::ostream &http::operator<<(std::ostream &os, const http::Response &response) {
    assert(!response.header.empty());
    for (auto datum: response.header) {
        os << datum.first << ": " << datum.second << std::endl;
    }
    os << response.cookies << std::endl;
    os << std::endl;
    os << response.content;
    return os;
}
