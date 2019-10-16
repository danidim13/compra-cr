//
// Created by daniel on 16/09/19.
//

#ifndef APP_ECOMMERCE_PRODUCTADDBUILDER_H
#define APP_ECOMMERCE_PRODUCTADDBUILDER_H

#include "../PageBuilder.h"
#include <map>

namespace view {

class ProductAddBuilder: public PageBuilder {
public:
    ProductAddBuilder(const std::string &title, const std::map<std::string, std::string> &errors);

    ProductAddBuilder(const std::string &title);

    std::string build_body() override;
    std::string title;
    std::map<std::string, std::string> errors;

};

}

#endif //APP_ECOMMERCE_PRODUCTADDBUILDER_H
