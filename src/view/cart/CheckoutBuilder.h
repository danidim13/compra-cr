//
// Created by daniel on 17/09/19.
//

#ifndef APP_ECOMMERCE_CHECKOUTBUILDER_H
#define APP_ECOMMERCE_CHECKOUTBUILDER_H


#include "../PageBuilder.h"
#include "../Table.h"
#include <map>

namespace view {

class CheckoutBuilder: public PageBuilder {
public:
    CheckoutBuilder(const std::string &title, const std::string &subtotal, const std::string &taxes,
                    const std::string &total, const Table &cartTable, const std::string &error);

    CheckoutBuilder(const std::string &title, const std::string &subtotal,
                    const std::string &taxes, const std::string &total,
                    const view::Table &cartTable, const std::string &error,
                    const std::map<std::string, std::string> &formErrors);

    CheckoutBuilder(const std::string &title, const std::string &subtotal,
                    const std::string &taxes, const std::string &total,
                    const view::Table &cartTable, const std::string &error,
                    const std::map<std::string, std::string> &formErrors,
                    const std::string &success);

private:
    std::string build_content() override;

public:

    std::string error;
    std::string subtotal;
    std::string taxes;
    std::string total;
    std::string success;
    Table cartTable;
    std::map<std::string, std::string> formErrors;

};

}

#endif //APP_ECOMMERCE_CHECKOUTBUILDER_H
