//
// Created by daniel on 03/09/19.
//

#include "Router.h"
#include "../util/log.h"
#include <string>

http::Router::Router() {
//    log_info(NULL, "Created Router");
}

void http::Router::parse_request() {
    std::string method(getenv("REQUEST_METHOD"));
    if (method.compare("GET") == 0) {
//        log_info(NULL, "GET request");
    } else if (method.compare("POST") == 0) {
//        log_info(NULL, "POST request");
    } else {
        std::string error_msg("Unrecognized request method: ");
        error_msg.append(method);
//        log_error(NULL, error_msg.c_str());
    }

}
