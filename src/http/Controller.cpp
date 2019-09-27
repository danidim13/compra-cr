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
            product_list_get();
        else if (req.m_Action.compare("/user/add") ==  0) {
            user_add_get();
        }
        else if (req.m_Action.compare("/product/add") == 0) {
            product_add_get();
        }
        else if (req.m_Action.compare("/user/login") == 0) {
            user_login_get();
        }
        else if (req.m_Action.compare("/user/logout") == 0) {
            user_logout_get();
        }
        else if (req.m_Action.compare("/cart/add") == 0) {
            cart_add_get();
        }
        else if (req.m_Action.compare("/cart/checkout") == 0) {
            cart_checkout_get();
        }
        else if (req.m_Action.compare("/cart/clear") == 0) {
            cart_clear_get();
        }
        else {
            // TODO: return 404 not found

        }

    } else if (req.m_Method.compare("POST") == 0) {
        // TODO: lista de acciones post
        if (req.m_Action.compare("/user/add") == 0) {
            user_add_post();
        }
        else if (req.m_Action.compare("/user/login") == 0) {
            user_login_post();
        }
        else if (req.m_Action.compare("/product/add") == 0) {
            product_add_post();
        }
        else if (req.m_Action.compare("/cart/checkout") == 0) {
            cart_checkout_post();
        }

    } else {
        // wtf
    }

    std::cout << *(get_router()->get_response()) << std::endl;
}

void http::Controller::product_list_get() {

    Request req = router->get_request();
    Response *resp = router->get_response();
    std::vector<model::Product> itemList;
    std::list<view::ProductCard> cards;
    std::string title;

    auto it = req.m_queryMap.find("search");

    if (it != req.m_queryMap.end() && !it->second.empty()) {
        // Hubo una búsqueda
        title = "Resultado de búsqueda";
        itemList = model::Product::search(it->second);
        for (auto item: itemList) {
            cards.push_back(view::ProductCard(item.id(), item.title(), item.detail(), item.unit_price()));
        }
    } else {
        // No hay búsqueda, listar los 6 más recientes
        title = "Lo último";
        itemList = model::Product::getLatestN(6);
        for (auto item: itemList) {
            cards.push_back(view::ProductCard(item.id(), item.title(), item.detail(), item.unit_price()));
        }
    }

    view::ProductListBuilder pageBuilder(title, cards);
    resp->header["Content-type"] = "text/html; charset=utf-8";
    resp->content = pageBuilder.build_document();
//    std::cout << "Content-type: text/html; charset=utf-8\n\n" << pageBuilder.build_document() << std::endl;
}

void http::Controller::product_add_get() {

    Request req = router->get_request();
    Response *resp = router->get_response();

    view::ProductAddBuilder pageBuilder("Agregar producto al catálogo");

    unsigned int user_id = strtoul(req.m_CookieMap["user_id"].c_str(), NULL, 10);
    if (user_id > 0 ) {
        resp->header["Content-type"] = "text/html; charset=utf-8";
        resp->content = pageBuilder.build_document();
//        std::cout << "Content-type: text/html; charset=utf-8\n\n" <<  pageBuilder.build_document() << std::endl;
    } else {
        resp->header["Status"] = "303 See Other";
        resp->header["Location"] = std::string("/user/login?error=Debe+loggearse+para+agregar+productos");
    }
}

void http::Controller::product_add_post() {

    Request req = router->get_request();
    Response *resp = router->get_response();


    unsigned int user_id = strtoul(req.m_CookieMap["user_id"].c_str(), NULL, 10);
    if (user_id > 0 ) {

        log_debug(NULL, (char *) "POST request en /product/add");
        log_debug(NULL, (char *) req.m_ContentType.c_str());
        log_debug(NULL, (char *) std::to_string(req.m_ContentLength).c_str());
        log_debug(NULL, (char *) req.m_Content.c_str());

        std::map<std::string, std::string> data = split_query((char *) req.m_Content.c_str());
        std::vector<std::string> select;

        data["owner_id"] = std::to_string(user_id);

        for (auto const &pair: data)
            select.push_back(pair.first);

//    for (auto datum: data) {
//        std::cout << datum.first << ": " << datum.second << std::endl;
//    }

        model::Product product;
        product.set_from_map(data);

        if (product.insert_autoId(select)) {
//        std::cout << "Exito" << std::endl;

            std::ostringstream msg;
            msg << "Created new product with title'" << product.title() << "'";
            log_info(NULL, (char *) msg.str().c_str());

            resp->header["Status"] = "302 Found";
            resp->header["Location"] = "/";
//        std::cout << "Status: 302 Found\n" << "Location: /\n\n";
        } else {
            resp->header["Content-type"] = "text/html; charset=utf-8";
            resp->content = "Error";
//        std::cout << "Content-type: text/html; charset=utf-8\n\n" << "Error" << std::endl;
        }
    } else {

        resp->header["Status"] = "303 See Other";
        resp->header["Location"] = std::string("/user/login?error=Debe+loggearse+para+agregar+productos");
    }

}

void http::Controller::user_add_get() {
    Response *resp = router->get_response();

    view::UserAddBuilder pageBuilder("Registrarse");
    resp->header["Content-type"] = "text/html; charset=utf-8";
    resp->content = pageBuilder.build_document();
//    std::cout << "Content-type: text/html; charset=utf-8\n\n" <<  pageBuilder.build_document() << std::endl;
}

void http::Controller::user_add_post() {

    Request req = router->get_request();
    Response *resp = router->get_response();


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

//        std::cout << "Status: 302 Found\n" << "Location: /\n\n";
        resp->header["Status"] = "302 Found";
        resp->header["Location"] = "/";
//        std::cout << *resp;

    } else {
        resp->header["Content-type"] = "text/html; charset=utf-8";
        resp->content = "Error";
//        std::cout << "Content-type: text/html; charset=utf-8\n\n" << "Error" << std::endl;
    }

}

void http::Controller::user_login_get() {

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

void http::Controller::user_login_post() {

    Request req = router->get_request();
    Response *resp = router->get_response();


    log_debug(NULL, (char*)"POST request en /user/login");
    log_debug(NULL, (char*)req.m_ContentType.c_str());
    log_debug(NULL, (char*)std::to_string(req.m_ContentLength).c_str());
    log_debug(NULL, (char*)req.m_Content.c_str());

    std::map<std::string, std::string> data = split_query((char*)req.m_Content.c_str());

    std::ostringstream cookie, msg;

    auto it = data.find("username");
    if (it != data.end()) {
        auto res = model::User::login(it->second);
        if (res.first) {
            // Login exitoso

//            msg << "Created new user with username '" << user.username() << "'";
//            log_info(NULL, (char*) msg.str().c_str());

            cookie << "user_id=" << res.second << "; Path=/;" << "Expires=" << renewed_time();

            resp->header["Status"] = "302 Found";
            resp->header["Set-Cookie"] = cookie.str();
            resp->header["Location"] = "/?login=success";

            /*
            std::cout << "Status: 302 Found\n"
                      << "Set-Cookie: user_id=" << res.second <<"; Path=/;" << "Expires=" << renewed_time() << "\n"
                      << "Location: /?login=success\n\n";
                      */
        } else {

            msg << "Error de login: " << res.second;
            log_debug(NULL, (char*) msg.str().c_str());
            resp->header["Status"] = "303 See Other";
            resp->header["Location"] = std::string("/user/login?error=").append(res.second);
//            std::cout << "Status: 303 See Other\n"
//                      << "Location: /user/login?error=" << res.second << "\n\n";

        }
    } else {
        // Error de formulario
        resp->header["Status"] = "303 See Other";
        resp->header["Location"] = std::string("/user/login?error=Por+favor+indique+su+usuario");
//        std::cout << "Status: 303 See Other\n"
//                  << "Location: /user/login?error=Por+favor+indique+su+usuario\n\n";

    }
}

void http::Controller::user_logout_get() {

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

void http::Controller::cart_add_get() {
    Request req = router->get_request();
    Response *resp = router->get_response();

    auto it = req.m_queryMap.find("id");

//    std::string cookie(req.m_HttpCookie);
    std::string items(req.m_CookieMap["shopping_cart"]);
    std::ostringstream new_cookie;


    if (it != req.m_queryMap.end()) {
        if (!items.empty()) {
            items.append(",");
        }
        items.append(it->second);
    }
    new_cookie << "shopping_cart=" << items << "; Path=/;" << "Expires=" << renewed_time();

    resp->header["Status"] = "302 Found";
    resp->header["Set-Cookie"] = new_cookie.str();
    resp->header["Location"] = "/";

//    std::cout << "Status: 302 Found\n"
//              << "Set-Cookie: shopping_cart=" << item << "; Path=/;" << "Expires=" << renewed_time() << "\n"
//              << "Location: /\n\n";

}

void http::Controller::cart_checkout_get() {

    Request req = router->get_request();
    Response *resp = router->get_response();
    std::vector<model::Product> products;

    unsigned int user_id = strtoul(req.m_CookieMap["user_id"].c_str(), NULL, 10);
    std::string items(req.m_CookieMap["shopping_cart"]);
    std::string error(req.m_queryMap["error"]);

    log_debug(NULL, (char*) "Procesando carrito de compras");
    log_debug(NULL, (char*) (std::string("user_id=").append(req.m_CookieMap["user_id"]).c_str()));
    log_debug(NULL, (char*) (std::string("shopping_cart=").append(req.m_CookieMap["shopping_cart"]).c_str()));


    if (user_id > 0) {
        if (!items.empty()) {

            //        log_debug(NULL, (char*)std::string("Products: ").append(items).c_str());
            products = model::Product::getItemsFromCart(items);
            double taxes = 0;
            double subtotal = 0;

            view::Table table({"Producto", "Cantidad", "Disponible", "Precio p/unidad"});
            for (auto product: products) {
                table.content.push_back(product.vector({"title", "amount", "stock", "unit_price"}));

                auto last = table.content.rbegin();
                if (product.amount() > product.stock()) {
                    (*last)[1] = std::move(std::string("<span class=\"text-danger\">").append(std::to_string(product.amount())).append("</span>\n"));
                    (*last)[2] = std::move(std::string("<span class=\"text-danger\">").append(std::to_string(product.stock())).append("</span>\n"));
                }

                double price = std::stod(product.unit_price());
                double tax_rate = std::stod(product.iva());
                // TODO: encapsular mejor el amount?
                subtotal += price*product.amount();
                taxes += price*tax_rate;
            }

            char price_buffer[256];
            char price_format[] = "%.2f";

            sprintf(price_buffer, price_format, subtotal);
            std::string ssubtotal(price_buffer);

            sprintf(price_buffer, price_format, taxes);
            std::string staxes(price_buffer);

            sprintf(price_buffer, price_format, taxes+subtotal);
            std::string total(price_buffer);

            view::CheckoutBuilder pageBuilder("Su orden", ssubtotal, staxes, total, table, error);

            resp->header["Content-type"] = "text/html; charset=utf-8";
            resp->content = pageBuilder.build_document();
        } else {
            view::Table table({"Producto", "Cantidad", "Disponible", "Precio p/unidad"});
            table.content.push_back({"Aún no tiene nada en su carrito", "", "", ""});
            view::CheckoutBuilder pageBuilder("Su orden", "0", "0", "0", table, error);

            resp->header["Content-type"] = "text/html; charset=utf-8";
            resp->content = pageBuilder.build_document();
        }
    } else {
        resp->header["Status"] = "303 See Other";
        resp->header["Location"] = std::string("/user/login?error=Debe+loggearse+para+hacer+una+compra");
    }
}

void http::Controller::cart_checkout_post() {

    Request req = router->get_request();
    Response *resp = router->get_response();
    std::vector<model::Product> products;

    std::ostringstream new_cookie;

    unsigned int user_id = strtoul(req.m_CookieMap["user_id"].c_str(), NULL, 10);
    std::string items(req.m_CookieMap["shopping_cart"]);

    std::map<std::string, std::string> card_data = split_query((char*)req.m_Content.c_str());

    log_debug(NULL, (char*) "Procesando venta");
    log_debug(NULL, (char*) (std::string("user_id=").append(req.m_CookieMap["user_id"]).c_str()));
    log_debug(NULL, (char*) (std::string("shopping_cart=").append(req.m_CookieMap["shopping_cart"]).c_str()));

    log_debug(NULL, (char*) "Información de tarjeta:");
    for (auto datum: card_data) {
        log_debug(NULL, (char*) std::string("").append(datum.first).append(": ").append(datum.second).c_str());
    }

    if (user_id > 0) {
        if (!items.empty()) {

            products = model::Product::getItemsFromCart(items);

            // Verificar que se tienen suficientes productos en existencia
            bool available = true;
            for (auto product: products) {
                if (product.amount() > product.stock()) {
                    available = false;
                    break;
                }
            }


            if (available) {

                // Realizar el pago
                auto transaction_res = model::Purchase::processPurchase(products, user_id, card_data);

                if (transaction_res.first) {
                    // Exito

                    // Expirar el cookie poco después
                    new_cookie << "shopping_cart=" << items << "; Path=/;" << "Expires=" << renewed_time(0, 1);
                    resp->header["Set-Cookie"] = new_cookie.str();

                    resp->header["Status"] = "302 Found";
                    resp->header["Location"] = std::string("/cart/checkout");
                } else {
                    // Fail

                    // Renovar el cookie
                    new_cookie << "shopping_cart=" << items << "; Path=/;" << "Expires=" << renewed_time(20, 0);
                    resp->header["Set-Cookie"] = new_cookie.str();

                    // Redirigir con error
                    resp->header["Status"] = "303 See Other";
                    resp->header["Location"] = std::string("/cart/checkout?error=").append(transaction_res.second);
                }

            } else {
                // Más items en el carrito de los disponibles

                // Renovar el cookie
                new_cookie << "shopping_cart=" << items << "; Path=/;" << "Expires=" << renewed_time(20, 0);
                resp->header["Set-Cookie"] = new_cookie.str();

                // Redirigir con error
                resp->header["Status"] = "303 See Other";
                resp->header["Location"] = std::string("/cart/checkout?error=Solicit%C3%B3+mas+productos+de+los+que+hay+disponibles");
            }

        } else {
            // Carrito vacío
            resp->header["Status"] = "303 See Other";
            resp->header["Location"] = std::string("/cart/checkout?error=Agregue+productos+para+realizar+su+compra");
        }
    } else {
        // Usuario no loggeado
        resp->header["Status"] = "303 See Other";
        resp->header["Location"] = std::string("/user/login?error=Debe+loggearse+para+hacer+una+compra");
    }
}

void http::Controller::cart_clear_get() {

    Response *resp = router->get_response();
    std::ostringstream cookie;

    cookie << "shopping_cart=none;" <<"; Path=/;" << "Expires=" << expired_time();

    resp->header["Status"] = "302 Found";
    resp->header["Set-Cookie"] = cookie.str();
    resp->header["Location"] = "/cart/checkout";
}
