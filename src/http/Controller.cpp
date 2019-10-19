//
// Created by daniel on 14/09/19.
//

#include <sstream>
#include "Controller.h"
#include "../model/model.h"
#include "../model/Product.h"
#include "../model/Purchase.h"
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
#include "../util/cookie.h"
#include "../view/Table.h"
#include "../view/cart/CheckoutBuilder.h"
#include "../validate/StringValidator.h"
#include "UserController.h"
#include "ProductController.h"
#include "CartController.h"

http::Controller::Controller() {
    router = http::get_router();
}

void http::Controller::processAction() {

    // Obtener y procesar el request
    validateReq();

    // Obtener información de sesión o generar una nueva
    setSession();

    processReq(*(router->get_request()));

    // Settear cookie
    refreshSession();

    // Escupir el resultado
    makeResponse();
}

void http::Controller::validateReq() {
    router->parse_request();
    Request *req = router->get_request();
    if (!req->valid) {
        // TODO: return 500 internal server error
        log_warning(NULL, "Invalid request");
        exit(1);
    }

}

void http::Controller::setSession() {

    Request *req = router->get_request();

    // Validación/sanitización implícita de user_id en la conversión
    if (req->m_CookieMap.find("user_id") != req->m_CookieMap.end()) {
        sessionManager.setUser(strtoul(req->m_CookieMap["user_id"].c_str(), NULL, 10));
    } else {
        sessionManager.setUser(0);
    }

    if (req->m_CookieMap.find("shopping_cart") != req->m_CookieMap.end()) {
        std::string items(req->m_CookieMap["shopping_cart"]);
        validate::StringValidator itemsValidator(validate::REGEX_CART_ITEMS, 1, 100);
        if (itemsValidator.validate(items).first) {
            sessionManager.setShoppingCart(items);
        }
    }

}

void http::Controller::processReq(const http::Request &request) {
    // Obtener y ejecutar la acción correspondiente
    if (request.m_Method.compare("GET") == 0) {
        processGetReq(request);
    } else if (request.m_Method.compare("POST") == 0) {
        processPostReq(request);
    } else {
        // wtf
    }

}

void http::Controller::refreshSession() {
    Response * response = router->get_response();
    response->cookies = sessionManager.getCookie();
}

void http::Controller::processGetReq(const Request &request) {


    // ProductController
    if (request.m_Action.compare("/product/list") == 0 ||
        request.m_Action.compare("/product/index") == 0 ||
        request.m_Action.compare("/") == 0 )

        ((ProductController*) this)->product_list_get();

    else if (request.m_Action.compare("/product/add") == 0) {

        ((ProductController*) this)->product_add_get();

    }

    // UserController
    else if (request.m_Action.compare("/user/add") ==  0) {

        ((UserController*) this)->user_add_get();

    } else if (request.m_Action.compare("/user/login") == 0) {

        ((UserController*) this)->user_login_get();

    } else if (request.m_Action.compare("/user/logout") == 0) {

        ((UserController*) this)->user_logout_get();

    }

    // CartController
    else if (request.m_Action.compare("/cart/add") == 0) {

        ((CartController*) this)->cart_add_get();

    } else if (request.m_Action.compare("/cart/checkout") == 0) {

        ((CartController*) this)->cart_checkout_get();

    } else if (request.m_Action.compare("/cart/clear") == 0) {

        ((CartController*) this)->cart_clear_get();

    }

    // Sin acción
    else {
        // TODO: return 404 not found

    }
}

void http::Controller::processPostReq(const http::Request &req) {

    // UserController
    if (req.m_Action.compare("/user/add") == 0) {
        ((UserController*) this)->user_add_post();
    }
    else if (req.m_Action.compare("/user/login") == 0) {
        ((UserController*) this)->user_login_post();
    }

    // ProductController
    else if (req.m_Action.compare("/product/add") == 0) {
        ((ProductController*) this)->product_add_post();
    }
    else if (req.m_Action.compare("/cart/checkout") == 0) {
        ((CartController*) this)->cart_checkout_post();
    }

    // Sin acción
    else {
        // TODO: return 404 not found
    }

}

void http::Controller::makeResponse() {
    if (pageView) {

        if (sessionManager.getUser() > 0) {
            pageView->setUserInfo("bla", 0);
        }

        Response *resp = router->get_response();
        resp->header["Content-type"] = "text/html; charset=utf-8";
        resp->content = pageView->build_document();
    }
    std::cout << *(router->get_response()) << std::endl;
}

void http::Controller::Found(std::string location) {
    Response *resp = router->get_response();
    resp->header["Status"] = "302 Found";
    resp->header["Location"] = location;
}

void http::Controller::SeeOther(std::string location) {
    Response *resp = router->get_response();
    resp->header["Status"] = "303 See Other";
    resp->header["Location"] = location;
}

void http::Controller::BadRequest(std::string location){
    Response *resp = router->get_response();
    resp->header["Status"] = "400 Bad Request";
    resp->header["Location"] = "/";
}



