//
// Created by daniel on 14/09/19.
//

#ifndef APP_ECOMMERCE_CONTROLLER_H
#define APP_ECOMMERCE_CONTROLLER_H


#include <string>
#include "Router.h"

#define LATEST 10

namespace http {

class Controller {
public:
    Controller();

    void processAction();
    void user_add_get();
    void user_add_post();
    void product_list();

private:
    Router *router;

};

}

#endif //APP_ECOMMERCE_CONTROLLER_H
