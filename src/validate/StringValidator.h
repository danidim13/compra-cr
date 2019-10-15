//
// Created by daniel on 15/10/19.
//

#ifndef APP_ECOMMERCE_STRINGVALIDATOR_H
#define APP_ECOMMERCE_STRINGVALIDATOR_H

#include "Validator.h"

namespace validate {

class StringValidator : public Validator {
public:
    StringValidator(const std::regex &format, int minLength, int maxLength);

    std::pair<bool, std::string> validate(const std::string &src) override;

private:
    std::regex format;
    int minLength;
    int maxLength;

};

}


#endif //APP_ECOMMERCE_STRINGVALIDATOR_H
