//
// Created by daniel on 14/09/19.
//

#include "Product.h"
#include <sstream>
#include "ConnectionHandler.h"
#include "../util/cookie.h"
#include "../validate/StringValidator.h"

model::Product::Product()
{
    table = "products";
    m_cols = {
        {"id", EntityAttr(0u)},
        {"owner_id", EntityAttr(0u)},
        {"title", EntityAttr("")},
        {"detail", EntityAttr("")},
        {"iva", EntityAttr("")},
        {"service_rate", EntityAttr("")},
        {"unit_price", EntityAttr("")},
        {"stock", EntityAttr(0u)},
    };

    default_validator.fields = {
        {"owner_id", std::make_shared<validate::StringValidator>(
                validate::REGEX_NUMBER, 1, 10 )},
        {"title", std::make_shared<validate::StringValidator>(
                validate::REGEX_SPANISH_NAMES, 1, 50 )},
        {"detail", std::make_shared<validate::StringValidator>(
                validate::REGEX_SPANISH_SENTENCE, 0, 500 )},
        {"unit_price", std::make_shared<validate::StringValidator>(
                validate::REGEX_PHONE, 1, 15 )},
        {"stock", std::make_shared<validate::StringValidator>(
                validate::REGEX_NUMBER, 1, 10 )},
    };
}

unsigned int model::Product::id() {
    return m_cols["id"].attrValue.u;
}

unsigned int model::Product::owner_id() {
    return m_cols["owner_id"].attrValue.u;
}

std::string model::Product::title() {
    return m_cols["title"].attrStr;
}

std::string model::Product::detail() {
    return m_cols["detail"].attrStr;
}

std::string model::Product::iva() {
    return m_cols["iva"].attrStr;
}

std::string model::Product::service_rate() {
    return m_cols["service_rate"].attrStr;
}

std::string model::Product::unit_price() {
    return m_cols["unit_price"].attrStr;
}

unsigned int model::Product::stock() {
    return m_cols["stock"].attrValue.u;
}

std::vector<model::Product> model::Product::getLatestN(unsigned int n) {

    std::ostringstream query;
    query << "SELECT * FROM products ORDER BY id DESC LIMIT " << n;
    std::vector<model::Product> products(model::ConnectionHandler::executeQuery<model::Product>(query.str()));
    return products;
}

std::vector<model::Product> model::Product::search(std::string search) {
    std::ostringstream query;
    query << "SELECT * FROM products WHERE ("
          << "title LIKE '%" << search << "%' OR "
          << "detail LIKE '%" << search << "%') "
          << "ORDER BY id DESC";

    std::vector<model::Product> products(model::ConnectionHandler::executeQuery<model::Product>(query.str()));
    return products;
}

std::vector<model::Product> model::Product::getItemsFromCart(std::string cart) {
    std::ostringstream query;
    std::map<unsigned int, int> itemMap = split_cart_str(cart.c_str());
//    std::vector<model::Product> entities;

    query << "SELECT * FROM products WHERE id IN (";
    for (auto it = itemMap.begin(); it != itemMap.end();) {
        query << it->first;
        if (++it != itemMap.end()) {
            query << ", ";
        }
    }
    query << ")";

    log_debug(NULL, (char*) std::string("Query: ").append(query.str()).c_str());

    std::vector<model::Product> entities(model::ConnectionHandler::executeQuery<model::Product>(query.str()));
    for (auto it = entities.begin(); it != entities.end(); ++it) {
        it->m_cols.emplace("amount", EntityAttr(itemMap[it->id()]));
    }
    return entities;
}

// TODO: encapsular mejor?
int model::Product::amount() {
    return m_cols["amount"].attrValue.i;
}
