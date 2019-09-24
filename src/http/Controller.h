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
    void user_login_get();
    void user_login_post();
    void user_logout_get();
    void product_list_get();
    void product_add_get();
    void product_add_post();
    void cart_add_get();
    void cart_checkout_get();
    void cart_checkout_post();
    void cart_clear_get();

private:
    Router *router;

};

}

#endif //APP_ECOMMERCE_CONTROLLER_H
