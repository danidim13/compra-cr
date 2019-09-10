//
// Created by daniel on 03/09/19.
//

#include "Router.h"
#include "../util/log.h"
#include "http.h"
#include <string>
#include <vector>
#include <cstring>


http::Router::Router() {
//    log_info(NULL, "Created Router");
    http::gpPathInfo = getenv("PATH_INFO");
    http::gpMethod = getenv("REQUEST_METHOD");
    http::gpQueryString = getenv("QUERY_STRING");

    char sbuf[1024];
    sprintf(sbuf, "Request received\nRequest method: %s\nPath info: %s\nQuery string: %s\nargc: %d\n", gpMethod, gpPathInfo, gpQueryString, giArgc);
    log_debug(NULL, sbuf);
}

void http::Router::parse_request() {

    std::string method(http::gpMethod);

    if (method.compare("GET") == 0) {
        log_info(NULL, (char*)"GET request");
    } else if (method.compare("POST") == 0) {
        log_info(NULL, (char*)"POST request");
    } else {
        std::string error_msg("Unrecognized request method: ");
        error_msg.append(method);
        log_error(NULL, (char*)error_msg.c_str());
        return;
    }

    this->parse_query_string();

}

unsigned long http::Router::parse_query_string() {

    char sbuf[1024];

    std::string query(http::gpQueryString);

    if (query.length() > 0){
        sprintf(sbuf, "Query string len: %lu, contents: %s", query.length(), query.c_str());
        log_debug(NULL, sbuf);

        std::vector<std::string> tokens;

        char *query_cpy = strdup(query.c_str());
        char *token;

        token = strtok(query_cpy, "&");
        while (token != NULL) {
            tokens.push_back(std::string(token));
            token = strtok(NULL, "&");
        }

        free(query_cpy);

        log_debug(NULL, (char*)"Tokens:");

        for (auto it = tokens.begin(); it != tokens.end(); ++it) {

            char *token_cpy = strdup(it->c_str());

            log_debug(NULL, token_cpy);
            token = strtok(token_cpy, "=");
            std::string key(token);

            token = strtok(NULL, "=");
            if (token) {
                std::string value(token);
                m_queryMap[key] = value;
            } else {
                sprintf(sbuf, "Invalid format on query token \"%s\"", it->c_str());
                log_error(NULL, sbuf);
            }

            free(token_cpy);
        }

    }
    return m_queryMap.size();
}
