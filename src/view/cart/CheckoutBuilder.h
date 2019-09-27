//
// Created by daniel on 17/09/19.
//

#ifndef APP_ECOMMERCE_CHECKOUTBUILDER_H
#define APP_ECOMMERCE_CHECKOUTBUILDER_H


#include "../PageBuilder.h"
#include "../Table.h"

namespace view {

class CheckoutBuilder: public PageBuilder {
public:
    CheckoutBuilder(const std::string &title, const std::string &subtotal, const std::string &taxes,
                    const std::string &total, const Table &cartTable, const std::string &error);

private:
    std::string build_body() override;

public:

    std::string title;
    std::string error;
    std::string subtotal;
    std::string taxes;
    std::string total;
    Table cartTable;

};

}

#endif //APP_ECOMMERCE_CHECKOUTBUILDER_H
