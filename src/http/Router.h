//
// Created by daniel on 03/09/19.
//

#ifndef APP_ECOMMERCE_ROUTER_H
#define APP_ECOMMERCE_ROUTER_H

#include <map>
#include <set>
#include "Request.h"

namespace http {
    class Router {
    public:
        Router();
        void parse_request();
        Request &get_request();

    protected:

        /**
         * Parses query string arguments, stores mapped values on m_queryMap
         * @return Number of key-value pairs parsed;
         */
        unsigned long parse_query_string();

        http::Request m_request;

        std::map<std::string, std::string> m_aliases;
        std::set<std::string> m_getActions;
        std::set<std::string> m_postActions;
    };
}


#endif //APP_ECOMMERCE_ROUTER_H
