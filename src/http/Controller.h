//
// Created by daniel on 14/09/19.
//

#ifndef APP_ECOMMERCE_CONTROLLER_H
#define APP_ECOMMERCE_CONTROLLER_H


#include <string>

#define LATEST 10

namespace http {

class Controller {
public:

    void product_list(std::string query = "");

private:

};

}

#endif //APP_ECOMMERCE_CONTROLLER_H
