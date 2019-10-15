//
// Created by daniel on 15/10/19.
//

#include "Validator.h"
#include <regex>


bool validate::Validator::regex(const std::string &src, const std::regex &regex) {

    return std::regex_match(src, regex);

}

bool validate::Validator::length_bewteen(const std::string &src, const int &min, const int &max) {
    return src.length() <= max && src.length() >= min;
}
