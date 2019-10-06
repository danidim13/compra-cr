//
// Created by daniel on 15/09/19.
//

#ifndef APP_ECOMMERCE_FORMINPUT_H
#define APP_ECOMMERCE_FORMINPUT_H


#include <string>
#include <ostream>

namespace view {

class FormInput {
public:

    FormInput(const std::string &label, const std::string &name);
    FormInput(const std::string &label, const std::string &name, const std::string &placeholder);

    std::string to_string() const;

    friend std::ostream &operator<<(std::ostream &os, const FormInput &input);

    std::string label;
    std::string name;
    std::string placeholder;
};
std::ostream &operator<<(std::ostream &os, const FormInput &input);

}

#endif //APP_ECOMMERCE_FORMINPUT_H
