//
// Created by daniel on 16/10/19.
//

#ifndef APP_ECOMMERCE_CARTCONTROLLER_H
#define APP_ECOMMERCE_CARTCONTROLLER_H


#include <string>
#include "Controller.h"
#include "Router.h"

namespace http {

    class CartController : public Controller {

    public:
        void cart_add_get();

        void cart_checkout_get();

        void cart_checkout_post();

        void cart_clear_get();

    private:

        struct SaleData {
            std::string subtotal;
            std::string taxes;
            std::string total;
            view::Table table;
        };

        SaleData getSaleData(std::vector<model::Product> products);

};


}

#endif //APP_ECOMMERCE_CARTCONTROLLER_H
