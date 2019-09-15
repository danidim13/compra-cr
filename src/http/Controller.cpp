//
// Created by daniel on 14/09/19.
//

#include "Controller.h"
#include "../model/model.h"
#include "../model/Product.h"
#include "http.h"
#include "../view/PageBuilder.h"
#include "../view/ProductCard.h"
#include "../view/product/ProductListBuilder.h"
#include "../view/user/UserAddBuilder.h"

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
        else if (req.m_Action.compare("/user/add") ==  0) {
            user_add_get();
        }
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

    Request req = router->get_request();
    std::vector<model::Product> itemList;
    std::list<view::ProductCard> cards;
    std::string title;

    auto it = req.m_queryMap.find("search");

    if (it != req.m_queryMap.end()) {
        // Hubo una búsqueda
        title = "Resultado de búsqueda";
        itemList = model::Product::getLatestN(4);
        for (auto item: itemList) {
            cards.push_back(view::ProductCard(item.title(), item.detail(), item.unit_price()));
        }
    } else {
        // No hay búsqueda, listar los 6 más recientes
        title = "Lo último";
        itemList = model::Product::getLatestN(6);
        for (auto item: itemList) {
            cards.push_back(view::ProductCard(item.title(), item.detail(), item.unit_price()));
        }
    }

    view::ProductListBuilder pageBuilder(title, cards);
    std::cout << pageBuilder.build_document() << std::endl;
}

void http::Controller::user_add_get() {

    view::UserAddBuilder pageBuilder("Registrarse");
    std::cout <<  pageBuilder.build_document() << std::endl;
}

void http::Controller::user_add_post() {

}
