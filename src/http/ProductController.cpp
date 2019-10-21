//
// Created by daniel on 16/10/19.
//

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
#include "ProductController.h"

/**
 * Inputs:
 *  - ?search
 * Outputs:
 *  - itemList
 */
void http::ProductController::product_list_get() {

    Request *req = router->get_request();
    Response *resp = router->get_response();
    std::vector<model::Product> itemList;
    std::list<view::ProductCard> cards;
    std::string title;

    auto it = req->m_queryMap.find("search");
    validate::StringValidator validator(validate::REGEX_SPANISH_SENTENCE, 1, 30);

    if (it != req->m_queryMap.end() && !it->second.empty()) {
        // Hubo una búsqueda
        if (validator.validate(it->second).first) {
            // Búsqueda válida
            title = "Resultado de búsqueda";
            itemList = model::Product::search(it->second);
        } else {
            // Búsqueda inválida
            title = "Lo último";
            itemList = model::Product::getLatestN(6);
        }
    } else {
        // No hay búsqueda, listar los 6 más recientes
        title = "Lo último";
        itemList = model::Product::getLatestN(6);
    }

    for (auto item: itemList) {
        cards.push_back(view::ProductCard(item.id(), item.title(), item.detail(), item.unit_price()));
    }

    pageView.reset(new view::ProductListBuilder(title, cards));
}

/**
 * Inputs:
 *  - cookie[user_id]
 *
 * Outputs: NA
 *
 */
void http::ProductController::product_add_get() {

    unsigned int user_id = sessionManager.getUser();
    if (user_id > 0 ) {
        pageView.reset(new view::ProductAddBuilder("Agregar producto al catálogo", _form_error));
    } else {
        return SeeOther("/user/login?error=Debe+loggearse+para+agregar+productos");
    }
}

/**
 * Inputs:
 *  - cookie["user_id"]
 *  - form -> product
 *
 * Outputs: NA
 */
void http::ProductController::product_add_post() {

    Request *req = router->get_request();
    Response *resp = router->get_response();

    model::Product product;
    validate::MapValidator validator = product.get_defaultValidator();


    // Validación implícita al convertir a int
    unsigned int user_id = sessionManager.getUser();

    if (user_id > 0 ) {

        log_debug(NULL, (char *) "POST request en /product/add");
        log_debug(NULL, (char *) req->m_ContentType.c_str());
        log_debug(NULL, (char *) std::to_string(req->m_ContentLength).c_str());
        log_debug(NULL, (char *) req->m_Content.c_str());

        std::map<std::string, std::string> data = split_query((char *) req->m_Content.c_str());
        data["owner_id"] = std::to_string(user_id);
        auto result = validator.validate(data);

        if (result.valid) {

            // Campos que vamos a guardar
            std::vector<std::string> select;
            for (auto const &pair: data)
                select.push_back(pair.first);

            product.set_from_map(data);

            if (product.insert_autoId(select)) {

                std::ostringstream msg;
                msg << "Created new product with title'" << product.title() << "'";
                log_info(NULL, (char *) msg.str().c_str());

                return Found("/");
            } else {
                // FIXME: error de bd (dar info?)
                return BadRequest("/product/add");
            }
        } else {
            // Error de validación
            (*_SESSION)["form_error"] = result.errors;
            return SeeOther("/product/add");
        }
    } else {
        // No hay usuario loggeado
        return SeeOther("/user/login?error=Debe+loggearse+para+agregar+productos");
    }

}