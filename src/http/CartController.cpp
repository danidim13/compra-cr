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
    Request *req = router->get_request();

    auto it = req->m_queryMap.find("id");
    std::vector<unsigned int> items(sessionManager.getShoppingCart());

    validate::StringValidator idValidator(validate::REGEX_NUMBER, 1, 10);

    if (sessionManager.getUser() > 0) {
        if (it != req->m_queryMap.end() && idValidator.validate(it->second).first ) {
            items.push_back(strtol(it->second.c_str(), NULL, 10));
            sessionManager.setShoppingCart(items);
            return Found("/");
        } else {
            // Error
            return BadRequest("/");
        }
    } else {
        SeeOther("/user/login?error=Para+realizar+una+compra+debe+ingresar+al+sistema");
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

    Request *req = router->get_request();
    std::vector<model::Product> products;

    std::vector<unsigned int> items = sessionManager.getShoppingCart();
    std::string error(req->m_queryMap["error"]);
    validate::StringValidator errorValidator(validate::REGEX_SPANISH_SENTENCE, 0, 100);

    log_debug(NULL, (char*) "Procesando carrito de compras");

    if (sessionManager.getUser() > 0) {
        if (!items.empty()) {

            products = model::Product::getItemsFromCart(items);

            SaleData sale(getSaleData(products));

            if (!errorValidator.validate(error).first) {
                error = "Error: por favor intente de nuevo";
            }
            return pageView.reset(new view::CheckoutBuilder("Su orden", sale.subtotal, sale.taxes, sale.total, sale.table, error, _form_error));
        } else {
            view::Table table({"Producto", "Cantidad", "Disponible", "Precio p/unidad"});
            table.content.push_back({"Aún no tiene nada en su carrito", "", "", ""});
            return pageView.reset(new view::CheckoutBuilder("Su orden", "0", "0", "0", table, error, _form_error));
        }
    } else {
        return SeeOther("/user/login?error=Debe+loggearse+para+hacer+una+compra");
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

    Request *req = router->get_request();
    Response *resp = router->get_response();

    std::vector<model::Product> products;
    std::ostringstream new_cookie;

    // Validación en sessionManager de user_id y shopping_cart
    unsigned int user_id = sessionManager.getUser();
    std::vector<unsigned int> items(sessionManager.getShoppingCart());

    // Validar datos de tarjeta
    std::map<std::string, std::string> card_data = split_query((char*)req->m_Content.c_str());
    validate::MapValidator cardValidator = model::Purchase::CardValidator();
    auto cardValRes = cardValidator.validate(card_data);

    log_debug(NULL, (char*) "Procesando venta");
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


            if (cardValRes.valid) {
                if (available) {

                    // Realizar el pago
                    auto transaction_res = model::Purchase::processPurchase(products, user_id, card_data);

                    if (transaction_res.first) {
                        // Expirar el cookie poco después
                        sessionManager.setShoppingCart(items, true);
                        return Found("/cart/checkout");
                    } else {
                        // Fail redirigir con error
                        return SeeOther(std::string("/cart/checkout?error=").append(transaction_res.second));
                    }

                } else {
                    // Redirigir con error
                    return SeeOther( "/cart/checkout?error=Solicit%C3%B3+mas+productos+de+los+que+hay+disponibles");
                }
            } else {

                log_info(NULL, "Datos de tarjeta inválidos");

                // Error en formulario de tarjeta
                (*_SESSION)["form_error"] = cardValRes.errors;
                std::string error("Error en formulario, revise sus datos");
                SeeOther(std::string("/cart/checkout?error=").append(error));
            }
        } else {
            // Carrito vacío
            return SeeOther("/cart/checkout?error=Agregue+productos+para+realizar+su+compra");
        }
    } else {
        // Usuario no loggeado
        return SeeOther("/user/login?error=Debe+loggearse+para+hacer+una+compra");
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
    sessionManager.setShoppingCart({});
    return Found("/cart/checkout");
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

