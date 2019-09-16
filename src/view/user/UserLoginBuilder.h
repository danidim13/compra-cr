//
// Created by daniel on 16/09/19.
//

#ifndef APP_ECOMMERCE_USERLOGINBUILDER_H
#define APP_ECOMMERCE_USERLOGINBUILDER_H

#include <string>
#include "../PageBuilder.h"

namespace view {

class UserLoginBuilder: public PageBuilder {
public:
    UserLoginBuilder(const std::string &title, const std::string &error);

    std::string build_body() override;
    std::string title;
    std::string error;
};

}

#endif //APP_ECOMMERCE_USERLOGINBUILDER_H
