//
// Created by daniel on 15/10/19.
//

#ifndef APP_ECOMMERCE_VALIDATOR_H
#define APP_ECOMMERCE_VALIDATOR_H


#include <string>

namespace validate {

const std::string REGEX_DECIMAL(R"(^(\+|-)?[0-9]+(\.[0-9]+)?$)");
const std::string REGEX_SPANISH_NAMES(R"(^[a-zA-ZáéíóúÁÉÍÓÚñÑ\s]*$)");

bool validate_regex(const std::string src, const std::string regex);

class Validator {



};

}

#endif //APP_ECOMMERCE_VALIDATOR_H
