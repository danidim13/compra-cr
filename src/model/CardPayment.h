//
// Created by daniel on 26/09/19.
//

#ifndef APP_ECOMMERCE_CARDPAYMENT_H
#define APP_ECOMMERCE_CARDPAYMENT_H


#include <string>
#include <map>

namespace model {

class CardPayment {
public:
    static std::pair<bool, std::string> process(const std::string &total, const std::map<std::string, std::string> &card_data);

};

}

#endif //APP_ECOMMERCE_CARDPAYMENT_H
