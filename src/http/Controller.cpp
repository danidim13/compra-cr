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
#include "../view/other/NotFoundPageBuilder.h"

http::Controller::Controller() {
    router = http::get_router();
}

void http::Controller::processAction() {

    // Obtener y procesar el request
    if (validateReq()) {
        // Obtener información de sesión o generar una nueva
        setSession();

        processReq(*(router->get_request()));

        // Settear cookie
        refreshSession();
    } else {
        log_warning(NULL, "Invalid request");
        BadRequest();
    }

    // Escupir el resultado
    makeResponse();
}

bool http::Controller::validateReq() {
    router->parse_request();
    Request *req = router->get_request();
    return req->valid;
}

void http::Controller::setSession() {

    Request *req = router->get_request();
    sessionManager.initFromCookie(req->m_CookieMap);

    _SESSION = &sessionManager.session_data;

    // Consumir error de formulario si existe
    if (_SESSION->find("form_error") != _SESSION->end() && (*_SESSION)["form_error"].is_object()) {
        _form_error = (*_SESSION)["form_error"].get<std::map<std::string, std::string>>();
        if (!_form_error.empty()) {
            (*_SESSION)["form_error"] = nlohmann::json::object();
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
        return MethodNotAllowed();
    }

}

void http::Controller::refreshSession() {
    sessionManager.pushSessionData();

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
        return NotFound();
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
        return NotFound();
    }

}

void http::Controller::makeResponse() {
    if (pageView) {

        if (sessionManager.getUser() > 0) {
            pageView->setUserInfo("bla", (*_SESSION)["shopping_cart"].size());
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

void http::Controller::BadRequest(){
    Response *resp = router->get_response();
    resp->header["Status"] = "400 Bad Request";
}

void http::Controller::NotFound() {
    Response *resp = router->get_response();
    resp->header["Status"] = "404 Not Found";
    pageView.reset(new view::NotFoundPageBuilder());
}

void http::Controller::MethodNotAllowed() {
    Response *resp = router->get_response();
    resp->header["Status"] = "405 Method Not Allowed";

}



