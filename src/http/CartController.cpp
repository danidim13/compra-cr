//
// Created by daniel on 16/10/19.
//

#include "ProductController.h"
#include "UserController.h"
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
#include "CartController.h"

/**
 * inputs: ?id, cookie["shopping_cart"]
 * outputs: cookie["shopping_cart"]
 */
void http::CartController::cart_add_get() {
    Request req = router->get_request();
    Response *resp = router->get_response();

    auto it = req.m_queryMap.find("id");
    std::string items(req.m_CookieMap["shopping_cart"]);
    std::ostringstream new_cookie;

    validate::StringValidator idValidator(validate::REGEX_NUMBER, 1, 10);
    validate::StringValidator itemsValidator(validate::REGEX_CART_ITEMS, 0, 100);

    if (it != req.m_queryMap.end() && idValidator.validate(it->second).first &&
            itemsValidator.validate(items).first) {

        if (!items.empty()) {
            items.append(",");
        }

        items.append(it->second);
        new_cookie << "shopping_cart=" << items << "; Path=/;" << "Expires=" << renewed_time();
        resp->header["Set-Cookie"] = new_cookie.str();
        resp->header["Status"] = "302 Found";
        resp->header["Location"] = "/";
    } else {
        // Error
        resp->header["Status"] = "400 Bad Request";
        resp->header["Location"] = "/";
    }
}

/**
 * Inputs:
 *  - cookie["id"]
 *  - cookie["shopping_cart"]
 *  - ?error
 *
 * Outputs:
 *  - subtotal
 *  - taxes
 *  - total
 *  - table
 *  - error
 */
void http::CartController::cart_checkout_get() {

    Request req = router->get_request();
    Response *resp = router->get_response();
    std::vector<model::Product> products;

    // Validación/sanitización implícita de user_id en la conversión
    unsigned int user_id = strtoul(req.m_CookieMap["user_id"].c_str(), NULL, 10);
    std::string items(req.m_CookieMap["shopping_cart"]);
    std::string error(req.m_queryMap["error"]);

    validate::StringValidator itemsValidator(validate::REGEX_CART_ITEMS, 1, 100);
    validate::StringValidator errorValidator(validate::REGEX_SPANISH_SENTENCE, 0, 100);

    log_debug(NULL, (char*) "Procesando carrito de compras");
    log_debug(NULL, (char*) (std::string("user_id=").append(req.m_CookieMap["user_id"]).c_str()));
    log_debug(NULL, (char*) (std::string("shopping_cart=").append(req.m_CookieMap["shopping_cart"]).c_str()));

    if (user_id > 0) {
        if (!items.empty() && itemsValidator.validate(items).first) {

            products = model::Product::getItemsFromCart(items);

            SaleData sale(getSaleData(products));

            if (!errorValidator.validate(error).first) {
                error = "Error: por favor intente de nuevo";
            }

            view::CheckoutBuilder pageBuilder("Su orden", sale.subtotal, sale.taxes, sale.total, sale.table, error);

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

/**
 * Inputs:
 *  - cookie["user_id"]
 *  - cookie["shopping_cart"]
 *  - form -> card_data
 *
 * Outputs:
 *  - cookie["shopping_cart"]
 *
 * En caso de error en formulario
 *  - subtotal
 *  - taxes
 *  - total
 *  - table
 *  - error
 */
void http::CartController::cart_checkout_post() {

    Request req = router->get_request();
    Response *resp = router->get_response();

    std::vector<model::Product> products;
    std::ostringstream new_cookie;

    // Validación impícita de user_id
    unsigned int user_id = strtoul(req.m_CookieMap["user_id"].c_str(), NULL, 10);
    std::string items(req.m_CookieMap["shopping_cart"]);
    std::map<std::string, std::string> card_data = split_query((char*)req.m_Content.c_str());

    validate::StringValidator itemsValidator(std::regex("^([0-9]+,)*[0-9]+$"), 1, 100);
    validate::MapValidator cardValidator = model::Purchase::CardValidator();

    auto cardValRes = cardValidator.validate(card_data);

    log_debug(NULL, (char*) "Procesando venta");
    log_debug(NULL, (char*) (std::string("user_id=").append(req.m_CookieMap["user_id"]).c_str()));
    log_debug(NULL, (char*) (std::string("shopping_cart=").append(req.m_CookieMap["shopping_cart"]).c_str()));

    log_debug(NULL, (char*) "Información de tarjeta:");
    for (auto datum: card_data) {
        log_debug(NULL, (char*) std::string("").append(datum.first).append(": ").append(datum.second).c_str());
    }

    if (user_id > 0) {
        if (!items.empty() && itemsValidator.validate(items).first) {

            products = model::Product::getItemsFromCart(items);

            // Verificar que se tienen suficientes productos en existencia
            bool available = true;
            for (auto product: products) {
                if (product.amount() > product.stock()) {
                    available = false;
                    break;
                }
            }


            if (cardValRes.valid) {
                if (available) {

                    // Realizar el pago
                    auto transaction_res = model::Purchase::processPurchase(products, user_id, card_data);

                    if (transaction_res.first) {

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
                    resp->header["Location"] = std::string(
                            "/cart/checkout?error=Solicit%C3%B3+mas+productos+de+los+que+hay+disponibles");
                }

            } else {

                // TODO: implementar con redirect
                log_info(NULL, "Datos de tarjeta inválidos");

                // Error en formulario de tarjeta
                SaleData sale = getSaleData(products);
                std::string error("Error en formulario, revise sus datos");

                view::CheckoutBuilder pageBuilder("Su orden", sale.subtotal, sale.taxes, sale.total, sale.table, error, cardValRes.errors);
                resp->header["Content-type"] = "text/html; charset=utf-8";
                resp->content = pageBuilder.build_document();
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

/**
 * Inputs: NA
 * Outputs:
 *  - cookie["shopping_cart"]
 */
void http::CartController::cart_clear_get() {

    Response *resp = router->get_response();
    std::ostringstream cookie;

    cookie << "shopping_cart=none;" <<"; Path=/;" << "Expires=" << expired_time();

    resp->header["Status"] = "302 Found";
    resp->header["Set-Cookie"] = cookie.str();
    resp->header["Location"] = "/cart/checkout";
}

http::CartController::SaleData http::CartController::getSaleData(std::vector<model::Product> products) {
    SaleData sale;
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

    sale.table = std::move(table);

    char price_buffer[256];
    char price_format[] = "%.2f";

    sprintf(price_buffer, price_format, subtotal);
    sale.subtotal = std::string(price_buffer);
    sprintf(price_buffer, price_format, taxes);
    sale.taxes = std::string(price_buffer);
    sprintf(price_buffer, price_format, taxes+subtotal);
    sale.total = std::string(price_buffer);

    return sale;
}

