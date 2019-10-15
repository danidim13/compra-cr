//
// Created by daniel on 15/10/19.
//

#ifndef APP_ECOMMERCE_MAPVALIDATOR_H
#define APP_ECOMMERCE_MAPVALIDATOR_H


#include <string>
#include <map>
#include "Validator.h"

namespace validate {

    class MapValidatorResult {
    public:
        bool valid;
        std::map<std::string, std::string> errors;
    };

    class MapValidator {
    public:
        typedef std::shared_ptr<Validator> ptrValidator_t;


        MapValidator(const std::map<std::string, ptrValidator_t> &fields);

        MapValidatorResult validate(const std::map<std::string, std::string> &raw_data);
        std::map<std::string, ptrValidator_t> fields;

    };

}

#endif //APP_ECOMMERCE_MAPVALIDATOR_H
