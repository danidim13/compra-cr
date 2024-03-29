//
// Created by daniel on 15/09/19.
//

#ifndef APP_ECOMMERCE_PRODUCTCARD_H
#define APP_ECOMMERCE_PRODUCTCARD_H

#include <string>
#include <ostream>
#include "../model/Product.h"

namespace view {

class ProductCard {

public:

    ProductCard();
    ProductCard(const unsigned int &id, const std::string &title, const std::string &detail, const std::string &price);

    std::string to_string() const;

    friend std::ostream &operator<<(std::ostream &os, const ProductCard &card);

    unsigned int id;
    std::string title;
    std::string detail;
    std::string price;
};
std::ostream &operator<<(std::ostream &os, const ProductCard &card);

}

#endif //APP_ECOMMERCE_PRODUCTCARD_H
