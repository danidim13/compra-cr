//
// Created by daniel on 15/10/19.
//

#ifndef APP_ECOMMERCE_VALIDATOR_H
#define APP_ECOMMERCE_VALIDATOR_H


#include <string>
#include <regex>

namespace validate {

    const std::regex REGEX_NUMBER(R"(^[0-9]+$)");
    const std::regex REGEX_CART_ITEMS(R"(^([0-9]+,)*([0-9]+)$)");
    const std::regex REGEX_DECIMAL(R"(^(\+|-)?[0-9]+(\.[0-9]+)?$)");
    const std::regex REGEX_SPANISH_NAMES(R"(^[a-zA-ZáéíóúüÁÉÍÓÚÜñÑ\s]*$)");
    const std::regex REGEX_USERNAME(R"(^[a-zA-Z0-9_.]+$)");
    const std::regex REGEX_SPANISH_SENTENCE(R"(^[a-zA-ZáéíóúüñÁÉÍÓÚÜÑ\s\.,!?¿¡;:]*$)");
    const std::regex REGEX_EMAIL(R"(^[a-zA-Z0-9_.]{1,64}@[a-zA-Z0-9_.]{1,255}$)");
    const std::regex REGEX_PHONE(R"(^(\+?([0-9]{1,4}|\([0-9]{1,4}\)) ?)?([0-9]+-)*[0-9]+$)");

    const std::regex REGEX_CARD_HOLDER(R"(^([a-zA-Z]+ ?){1,4}$)");
    const std::regex REGEX_CREDIT_CARD(R"(^[0-9]{4}(-[0-9]{4}){3}$)");
    const std::regex REGEX_CCV(R"(^[0-9]{3,5}$)");
    const std::regex REGEX_EXPIRE(R"(^(1[0-2]|0[1-9])/[0-9]{2}$)");
    const std::regex REGEX_B64(R"(^([A-Za-z0-9+\/]{4})*([A-Za-z0-9+\/]{4}|[A-Za-z0-9+\/]{3}=|[A-Za-z0-9+\/]{2}==)$)");
    const std::regex REGEX_PASSWORD(R"((?=.*[A-Z])(?=.*[a-z])(?=.*\d)[A-Za-z\d]{8,})");


    class Validator {

    public:
        virtual std::pair<bool, std::string> validate(const std::string &src) = 0;

        static bool regex(const std::string &src, const std::regex &regex);
        static bool length_bewteen(const std::string &src, const int &min, const int &max);

    };

}

#endif //APP_ECOMMERCE_VALIDATOR_H
