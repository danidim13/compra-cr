//
// Created by daniel on 26/09/19.
//

#include "CardPayment.h"
#include "../util/log.h"
#include <cstdlib>
#include <ctime>

std::pair<bool, std::string>
model::CardPayment::process(const std::string &total, const std::map<std::string, std::string> &card_data) {

    std::pair<bool, std::string> result;

    log_debug(NULL, (char*) "Tirando el dado para el pago");


    srand(time(NULL));
    if ((double)rand() < 0.66 * RAND_MAX && card_data.find("card_number")->second.size() >= 4) {

        log_debug(NULL, (char*) "Pago exitoso");

        auto it = card_data.find("card_number");
        std::string last4_digits(it->second.substr(it->second.size()-4, 4));

        result.first = true;
        result.second = std::string("FE-").append(last4_digits).append("-").append(std::to_string(rand()%1000000));
//        result.second = std::string("GOOD");
    } else {
        log_debug(NULL, (char*) "Pago denegado");
        result.first = false;
        result.second = std::string("Fondos insuficientes");
    }
    return result;
}
