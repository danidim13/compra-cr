//
// Created by daniel on 15/09/19.
//

#ifndef APP_ECOMMERCE_FORM_H
#define APP_ECOMMERCE_FORM_H


#include <string>
#include <vector>
#include <ostream>
#include "DomItem.h"
#include "FormInput.h"

namespace view {

class Form {
public:
    std::string method;
    std::string action;
    std::string submit_label;
    std::vector<FormInput> inputs;

    Form(const std::string &method, const std::string &action);
    Form(const std::string &method, const std::string &action, const std::vector<FormInput> &inputs);
    Form(const std::string &method, const std::string &action, const std::string &submitLabel,
         const std::vector<FormInput> &inputs);


    std::string to_string() const;

    friend std::ostream &operator<<(std::ostream &os, const Form &form);

};

}

#endif //APP_ECOMMERCE_FORM_H
