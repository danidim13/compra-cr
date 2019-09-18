//
// Created by daniel on 16/09/19.
//

#ifndef APP_ECOMMERCE_COOKIE_H
#define APP_ECOMMERCE_COOKIE_H

#include <ctime>
#include <string>
#include <map>

//                 "Wed, 01 Aug 2019 13:00:00 GMT"
#define GMT_FORMAT "%a, %d %b %Y %T GMT"
#define GMT_LENGTH 30
#define SESSION_MIN 20

std::string format_cookie_time(const time_t &timer);
std::string format_cookie_time(struct tm *gmt);
std::string expired_time();
std::string renewed_time();
std::map<std::string, std::string> split_cookie(const char *src);
std::map<unsigned int, int> split_cart_str(const char *cart);
std::map<unsigned int, int> get_cart_items(const char *cookie);

#endif //APP_ECOMMERCE_COOKIE_H
