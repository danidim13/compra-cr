//
// Created by daniel on 15/09/19.
//

#ifndef APP_ECOMMERCE_USERADDBUILDER_H
#define APP_ECOMMERCE_USERADDBUILDER_H


#include "../PageBuilder.h"
#include "../ProductCard.h"


namespace view {

class UserAddBuilder: public PageBuilder {
public:
    UserAddBuilder(const std::string &title);

    std::string build_content() override;

    UserAddBuilder(const std::string &title, const std::map<std::string, std::string> &errors);

    std::map<std::string, std::string> errors;
};

}

#endif //APP_ECOMMERCE_USERADDBUILDER_H
