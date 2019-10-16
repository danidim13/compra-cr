//
// Created by daniel on 14/09/19.
//

#ifndef APP_ECOMMERCE_PURCHASE_H
#define APP_ECOMMERCE_PURCHASE_H

#include "model.h"
#include "Product.h"

namespace model {
class Purchase: public Entity {
public:
    Purchase();

    unsigned int id(); // int unsigned not null auto_increment primary key;
    unsigned int buyer_id(); // int unsigned not null;
    std::string order_date(); // datetime not null;
    std::string cost_tax(); // decimal(12,2) not null;
    std::string cost_raw(); // decimal(12,2) not null;
    std::string cost_total(); // decimal(12,2) not null;
    std::string payment_state(); // enum('retenido','confirmado','reclamo pendiente','devolución') not null default 'retenido'
    std::string receipt_code(); // char(14) null;

    static std::pair<bool, std::string>
    processPurchase(std::vector<Product> products, unsigned int buyer, std::map<std::string, std::string> card_data);

    static validate::MapValidator CardValidator();
};
}

#include <string>
#include "mysql_driver.h"

#endif //APP_ECOMMERCE_PURCHASE_H
