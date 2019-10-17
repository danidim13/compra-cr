//
// Created by daniel on 16/10/19.
//

#ifndef APP_ECOMMERCE_USERCONTROLLER_H
#define APP_ECOMMERCE_USERCONTROLLER_H

#include <string>
#include "Controller.h"
#include "Router.h"

namespace http {

class UserController : public Controller {

public:
    void user_add_get();

    void user_add_post();

    void user_login_get();

    void user_login_post();

    void user_logout_get();
};

}


#endif //APP_ECOMMERCE_USERCONTROLLER_H
