//
// Created by daniel on 14/09/19.
//

#ifndef APP_ECOMMERCE_PRODUCT_H
#define APP_ECOMMERCE_PRODUCT_H

#include "model.h"
#include <string>
#include <vector>

namespace model {
class Product: public Entity {
public:
    Product();

    unsigned int id(); // int unsigned not null auto_increment primary key;
    unsigned int owner_id(); // int unsigned not null;
    std::string title(); // varchar(50) not null;
    std::string detail(); // varchar(500) null;
    std::string iva(); // decimal(5,4) not null default 0.13;
    std::string service_rate(); // decimal(5,4) not null default 0.15;
    std::string unit_price(); // decimal(12,2) not null;
    unsigned int stock(); // int unsigned not null
    int amount(); // int unsigned not null

    static std::vector<Product> getLatestN(unsigned int n);
    static std::vector<Product> search(std::string keyword);
    static std::vector<Product> getItemsFromCart(std::vector<unsigned int> cart);
};
}


#endif //APP_ECOMMERCE_PRODUCT_H
