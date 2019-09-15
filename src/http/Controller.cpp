//
// Created by daniel on 14/09/19.
//

#include "Controller.h"
#include "../model/model.h"
#include "../model/Product.h"
#include "http.h"
#include "../view/PageBuilder.h"

http::Controller::Controller() {
    router = http::get_router();
}

void http::Controller::processAction() {
    router->parse_request();

    Request req = router->get_request();

    if (!req.valid) {
        // TODO: return 500 internal server error
    } else if (req.m_Method.compare("GET") == 0) {
        if (req.m_Action.compare("/product/list") == 0
                || req.m_Action.compare("/product/index") == 0
                || req.m_Action.compare("/") == 0 )
            product_list();

        else {
            // TODO: return 404 not found

        }

    } else if (req.m_Method.compare("POST") == 0) {
        // TODO: lista de acciones post


    } else {
        // wtf
    }
}

void http::Controller::product_list() {
    std::vector<model::Product> itemList;
    Request req = router->get_request();

    auto it = req.m_queryMap.find("search");

    if (it != req.m_queryMap.end()) {
        itemList = model::Product::getLatestN(6);
    } else {
        itemList = model::Product::getLatestN(6);
    }

    PageBuilder pageBuilder;
    std::cout << pageBuilder.build_document() << std::endl;
}
