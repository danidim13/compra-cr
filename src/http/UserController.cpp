//
// Created by daniel on 16/10/19.
//

#include "../validate/StringValidator.h"
#include "../view/cart/CheckoutBuilder.h"
#include "../view/Table.h"
#include "../util/cookie.h"
#include "../view/user/UserLoginBuilder.h"
#include "../view/product/ProductAddBuilder.h"
#include "../util/decode.h"
#include "../util/log.h"
#include "../view/user/UserAddBuilder.h"
#include "../view/product/ProductListBuilder.h"
#include "../view/ProductCard.h"
#include "../view/PageBuilder.h"
#include "http.h"
#include "../model/User.h"
#include "../model/Purchase.h"
#include "../model/Product.h"
#include "../model/model.h"
#include "Controller.h"
#include <sstream>
#include "UserController.h"

/**
 * Inputs: NA
 * Outputs: NA
 */
void http::UserController::user_add_get() {
    Response *resp = router->get_response();

    view::UserAddBuilder pageBuilder("Registrarse");
    resp->header["Content-type"] = "text/html; charset=utf-8";
    resp->content = pageBuilder.build_document();
//    std::cout << "Content-type: text/html; charset=utf-8\n\n" <<  pageBuilder.build_document() << std::endl;
}

/**
 * Inputs: form -> user
 * Outputs: NA
 */
void http::UserController::user_add_post() {

    Request req = router->get_request();
    Response *resp = router->get_response();


    log_debug(NULL, (char*)"POST request en /user/add");
    log_debug(NULL, (char*)req.m_ContentType.c_str());
    log_debug(NULL, (char*)std::to_string(req.m_ContentLength).c_str());
    log_debug(NULL, (char*)req.m_Content.c_str());

    std::map<std::string, std::string> data = split_query((char*)req.m_Content.c_str());
    model::User user;
    validate::MapValidator validator = user.get_defaultValidator();
    auto result = validator.validate(data);

    if (result.valid) {
        user.set_from_map(data);

        if (user.insert_autoId()) {
    //        std::cout << "Exito" << std::endl;

            std::ostringstream msg;
            msg << "Created new user with username '" << user.username() << "'";
            log_info(NULL, (char*) msg.str().c_str());

    //        std::cout << "Status: 302 Found\n" << "Location: /\n\n";
            resp->header["Status"] = "302 Found";
            resp->header["Location"] = "/";
    //        std::cout << *resp;

        } else {
            resp->header["Content-type"] = "text/html; charset=utf-8";
            resp->content = "Error";
    //        std::cout << "Content-type: text/html; charset=utf-8\n\n" << "Error" << std::endl;
        }
    } else {
        log_debug(NULL, (char *) "Error de validacion");
        for (auto err : result.errors) {
            log_debug(NULL, (char*)err.second.c_str());
        }
        view::UserAddBuilder pageBuilder("Registrarse", result.errors);
        resp->header["Content-type"] = "text/html; charset=utf-8";
        resp->content = pageBuilder.build_document();
    }

}

/**
 * Inputs: NA
 * Outputs: NA
 */
void http::UserController::user_login_get() {

    Request req = router->get_request();
    Response *resp = router->get_response();
    std::string errors("");

    auto it = req.m_queryMap.find("error");
    if (it != req.m_queryMap.end()) {
        errors = it->second;
    }

    view::UserLoginBuilder pageBuilder("Acceder al sistema", errors);
    resp->header["Content-type"] = "text/html; charset=utf-8";
    resp->content = pageBuilder.build_document();
//    std::cout << "Content-type: text/html; charset=utf-8\n\n" <<  pageBuilder.build_document() << std::endl;

}

/**
 * Inputs:
 *  - form -> credentials
 * Outputs:
 *  -
 */
void http::UserController::user_login_post() {

    Request req = router->get_request();
    Response *resp = router->get_response();


    log_debug(NULL, (char*)"POST request en /user/login");
    log_debug(NULL, (char*)req.m_ContentType.c_str());
    log_debug(NULL, (char*)std::to_string(req.m_ContentLength).c_str());
    log_debug(NULL, (char*)req.m_Content.c_str());

    std::map<std::string, std::string> data = split_query((char*)req.m_Content.c_str());
    validate::StringValidator validator(validate::REGEX_USERNAME, 1, 50);

    std::ostringstream cookie, msg;

    auto it = data.find("username");
    if (it != data.end()) {
        auto vResult = validator.validate(it->second);

        if (vResult.first) {
            auto res = model::User::login(it->second);
            if (res.first) {
                // Login exitoso

                cookie << "user_id=" << res.second << "; Path=/;" << "Expires=" << renewed_time();

                resp->header["Status"] = "302 Found";
                resp->header["Set-Cookie"] = cookie.str();
                resp->header["Location"] = "/?login=success";

            } else {
                msg << "Error de login: " << res.second;
                log_debug(NULL, (char *) msg.str().c_str());
                resp->header["Status"] = "303 See Other";
                resp->header["Location"] = std::string("/user/login?error=Error%2C+verifique+sus+credenciales");
            }
        } else {
            // Error de formulario
            resp->header["Status"] = "303 See Other";
            resp->header["Location"] = std::string("/user/login?error=Error%2C+verifique+sus+credenciales");
        }
    } else {
        // Error de formulario
        resp->header["Status"] = "303 See Other";
        resp->header["Location"] = std::string("/user/login?error=Por+favor+indique+su+usuario");
    }
}

/**
 * Inputs: NA
 * Outputs: NA
 */
void http::UserController::user_logout_get() {

    Response *resp = router->get_response();
    std::ostringstream cookie;

    cookie << "user_id=none;" <<"; Path=/;" << "Expires=" << expired_time();

    resp->header["Status"] = "302 Found";
    resp->header["Set-Cookie"] = cookie.str();
    resp->header["Location"] = "/?logout=success";

//    std::cout << "Status: 302 Found\n"
//              << "Set-Cookie: user_id=none;" <<"; Path=/;" << "Expires=" << expired_time() << "\n"
//              << "Location: /?logout=success\n\n";
}