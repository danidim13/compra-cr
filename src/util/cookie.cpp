//
// Created by daniel on 16/09/19.
//

#include "cookie.h"

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
