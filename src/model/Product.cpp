//
// Created by daniel on 14/09/19.
//

#include "Product.h"
#include <sstream>
#include "ConnectionHandler.h"

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
