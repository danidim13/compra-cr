//
// Created by daniel on 15/10/19.
//

#ifndef APP_ECOMMERCE_EMAILVALIDATOR_H
#define APP_ECOMMERCE_EMAILVALIDATOR_H


#include "Validator.h"

namespace validate {

class EmailValidator : public Validator {
public:
    EmailValidator();
    std::pair<bool, std::string> validate(const std::string &src) override;

};

}

#endif //APP_ECOMMERCE_EMAILVALIDATOR_H
