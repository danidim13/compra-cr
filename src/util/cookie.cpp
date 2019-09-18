//
// Created by daniel on 16/09/19.
//

#include <vector>
#include <memory.h>
#include "cookie.h"
#include "log.h"

std::string format_cookie_time(const time_t &raw_time) {

    struct tm *pGmt;

    pGmt = gmtime(&raw_time);

    return format_cookie_time(pGmt);
}

std::string format_cookie_time(struct tm *gmt) {
    char time_buf[GMT_LENGTH];
    strftime(time_buf, GMT_LENGTH, GMT_FORMAT, gmt);
    return std::string(time_buf);
}

std::string renewed_time() {
    time_t now;
    struct tm *pLocal;

    time(&now);
    pLocal = localtime(&now);
    pLocal->tm_min += SESSION_MIN;

    return format_cookie_time(mktime(pLocal));
}


std::string expired_time() {
    time_t now;
    struct tm *pLocal;

    time(&now);
    pLocal = localtime(&now);
    pLocal->tm_year--;

    return format_cookie_time(mktime(pLocal));
}

std::map<std::string, std::string> split_cookie(const char *src) {
    std::vector<std::string> tokens;
    std::map<std::string, std::string> cookieMap;

    char *token;
    char *query_cpy = strdup(src);

    // Partir string en tokens por '; '
    token = strtok(query_cpy, "; ");
    while (token != NULL) {
        tokens.push_back(std::string(token));
        token = strtok(NULL, "; ");
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
//            url_decode(token, token);
            std::string value(token);
            cookieMap[key] = value;
        } else {
            char sbuf[1024];
            sprintf(sbuf, "Empty or invalid format on cookie token \"%s\"", it->c_str());
            log_warning(NULL, sbuf);
            cookieMap[key] = "";
        }

        free(token_cpy);
    }
    return cookieMap;
    return std::map<std::string, std::string>();
}

std::map<unsigned int, int> split_cart_str(const char *cart) {
    std::map<unsigned int, int> itemMap;
    char *token;
    char *cart_cpy;

    cart_cpy = strdup(cart);

    token = strtok(cart_cpy, ",");
    while (token != NULL) {
        itemMap[strtol(token, NULL, 10)] += 1;
        token = strtok(NULL, ",");
    }
    free(cart_cpy);

    return itemMap;
}

std::map<unsigned int, int> get_cart_items(const char *cookie) {
    auto cookieMap = split_cookie(cookie);
    return split_cart_str(cookieMap["shopping_cart"].c_str());
}
