//
// Created by daniel on 16/10/19.
//

#ifndef APP_ECOMMERCE_PRODUCTCONTROLLER_H
#define APP_ECOMMERCE_PRODUCTCONTROLLER_H

#include <string>
#include "Controller.h"
#include "Router.h"

namespace http {

class ProductController : public Controller {

public:
    void product_list_get();

    void product_add_get();

    void product_add_post();
};

}


#endif //APP_ECOMMERCE_PRODUCTCONTROLLER_H
