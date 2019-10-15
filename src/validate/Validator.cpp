//
// Created by daniel on 15/10/19.
//

#include "Validator.h"
#include <regex>

bool validate::validate_regex(const std::string src, const std::string regex) {

    return std::regex_match(src, std::regex(regex));

}
