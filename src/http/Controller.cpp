//
// Created by daniel on 14/09/19.
//

#include <sstream>
#include "Controller.h"
#include "../model/model.h"
#include "../model/Product.h"
#include "../model/User.h"
#include "http.h"
#include "../view/PageBuilder.h"
#include "../view/ProductCard.h"
#include "../view/product/ProductListBuilder.h"
#include "../view/user/UserAddBuilder.h"
#include "../util/log.h"
#include "../util/decode.h"
#include "../view/product/ProductAddBuilder.h"
#include "../view/user/UserLoginBuilder.h"

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
        else if (req.m_Action.compare("/product/add") == 0) {
            product_add_get();
        }
        else if (req.m_Action.compare("/user/login") == 0) {
            user_login_get();
        }
        else {
            // TODO: return 404 not found

        }

    } else if (req.m_Method.compare("POST") == 0) {
        // TODO: lista de acciones post
        if (req.m_Action.compare("/user/add") == 0) {
            user_add_post();
        }
        if (req.m_Action.compare("/user/login") == 0) {
            user_login_post();
        }
        else if (req.m_Action.compare("/product/add") == 0) {
            product_add_post();
        }

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

    if (it != req.m_queryMap.end() && !it->second.empty()) {
        // Hubo una búsqueda
        title = "Resultado de búsqueda";
        itemList = model::Product::search(it->second);
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
    std::cout << "Content-type: text/html; charset=utf-8\n\n" << pageBuilder.build_document() << std::endl;
}

void http::Controller::product_add_get() {

    view::ProductAddBuilder pageBuilder("Agregar producto al catálogo");
    std::cout << "Content-type: text/html; charset=utf-8\n\n" <<  pageBuilder.build_document() << std::endl;
}

void http::Controller::product_add_post() {

    Request req = router->get_request();


    log_debug(NULL, (char*)"POST request en /product/add");
    log_debug(NULL, (char*)req.m_ContentType.c_str());
    log_debug(NULL, (char*)std::to_string(req.m_ContentLength).c_str());
    log_debug(NULL, (char*)req.m_Content.c_str());

    std::map<std::string, std::string> data = split_query((char*)req.m_Content.c_str());
//    for (auto datum: data) {
//        std::cout << datum.first << ": " << datum.second << std::endl;
//    }

    model::Product product;
    product.set_from_map(data);

    if (product.insert_autoId()) {
//        std::cout << "Exito" << std::endl;

        std::ostringstream msg;
        msg << "Created new product with title'" << product.title() << "'";
        log_info(NULL, (char*) msg.str().c_str());

        std::cout << "Status: 302 Found\n" << "Location: /\n\n";
    } else {
        std::cout << "Content-type: text/html; charset=utf-8\n\n" << "Error" << std::endl;
    }

}

void http::Controller::user_add_get() {

    view::UserAddBuilder pageBuilder("Registrarse");
    std::cout << "Content-type: text/html; charset=utf-8\n\n" <<  pageBuilder.build_document() << std::endl;
}

void http::Controller::user_add_post() {

    Request req = router->get_request();


    log_debug(NULL, (char*)"POST request en /user/add");
    log_debug(NULL, (char*)req.m_ContentType.c_str());
    log_debug(NULL, (char*)std::to_string(req.m_ContentLength).c_str());
    log_debug(NULL, (char*)req.m_Content.c_str());

    std::map<std::string, std::string> data = split_query((char*)req.m_Content.c_str());
//    for (auto datum: data) {
//        std::cout << datum.first << ": " << datum.second << std::endl;
//    }

    model::User user;
    user.set_from_map(data);

    if (user.insert_autoId()) {
//        std::cout << "Exito" << std::endl;

        std::ostringstream msg;
        msg << "Created new user with username '" << user.username() << "'";
        log_info(NULL, (char*) msg.str().c_str());

        std::cout << "Status: 302 Found\n" << "Location: /\n\n";
    } else {
        std::cout << "Content-type: text/html; charset=utf-8\n\n" << "Error" << std::endl;
    }

}

void http::Controller::user_login_get() {

    Request req = router->get_request();
    std::string errors("");

    auto it = req.m_queryMap.find("error");
    if (it != req.m_queryMap.end()) {
        errors = it->second;
    }

    view::UserLoginBuilder pageBuilder("Acceder al sistema", errors);
    std::cout << "Content-type: text/html; charset=utf-8\n\n" <<  pageBuilder.build_document() << std::endl;

}

void http::Controller::user_login_post() {

    Request req = router->get_request();


    log_debug(NULL, (char*)"POST request en /user/login");
    log_debug(NULL, (char*)req.m_ContentType.c_str());
    log_debug(NULL, (char*)std::to_string(req.m_ContentLength).c_str());
    log_debug(NULL, (char*)req.m_Content.c_str());

    std::map<std::string, std::string> data = split_query((char*)req.m_Content.c_str());

    std::ostringstream msg;

    auto it = data.find("username");
    if (it != data.end()) {
        auto res = model::User::login(it->second);
        if (res.first) {
            // Login exitoso

//            msg << "Created new user with username '" << user.username() << "'";
//            log_info(NULL, (char*) msg.str().c_str());

            std::cout << "Status: 302 Found\n"
                      << "Set-Cookie: user_id=" << res.second <<"\n"
                      << "Location: /?login=success\n\n";


        } else {

            msg << "Error de login: " << res.second;
            log_debug(NULL, (char*) msg.str().c_str());
            std::cout << "Status: 303 See Other\n"
                      << "Location: /user/login?error=" << res.second << "\n\n";

        }
    } else {
        // Error de formulario
        std::cout << "Status: 303 See Other\n"
                  << "Location: /user/login?error=Por+favor+indique+su+usuario\n\n";

    }
}
