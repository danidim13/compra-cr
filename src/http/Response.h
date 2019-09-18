//
// Created by daniel on 17/09/19.
//

#ifndef APP_ECOMMERCE_RESPONSE_H
#define APP_ECOMMERCE_RESPONSE_H

#include <string>
#include <map>
#include <ostream>

namespace http {

class Response {
public:
    friend std::ostream &operator<<(std::ostream &os, const Response &response);
    std::map<std::string, std::string> header;
    std::string content;

};

}

#endif //APP_ECOMMERCE_RESPONSE_H
