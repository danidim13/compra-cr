//
// Created by daniel on 16/09/19.
//

#ifndef APP_ECOMMERCE_DECODE_H
#define APP_ECOMMERCE_DECODE_H

#include <string>
#include <map>

std::string url_decode(char *src);
size_t url_decode(char *src, char *dest);

std::map<std::string, std::string> split_query(char *src);

#endif //APP_ECOMMERCE_DECODE_H
