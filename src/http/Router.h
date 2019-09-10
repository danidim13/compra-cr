//
// Created by daniel on 03/09/19.
//

#ifndef APP_ECOMMERCE_ROUTER_H
#define APP_ECOMMERCE_ROUTER_H

#include <unordered_map>

namespace http {
    class Router {
    public:
        Router();
        void parse_request();

    private:

        /**
         * Parses query string arguments, stores mapped values on m_queryMap
         * @return Number of key-value pairs parsed;
         */
        unsigned long parse_query_string();

        std::unordered_map<std::string, std::string> m_queryMap;

    };
}


#endif //APP_ECOMMERCE_ROUTER_H
