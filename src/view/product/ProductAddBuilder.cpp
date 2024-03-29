//
// Created by daniel on 16/09/19.
//

#include <sstream>
#include "ProductAddBuilder.h"
#include "../Form.h"

view::ProductAddBuilder::ProductAddBuilder(const std::string &title) : PageBuilder(title) {}

std::string view::ProductAddBuilder::build_content() {
    std::ostringstream content;
    view::Form form("POST", "/product/add", {
        {"Título", "title", "p. ej. Olla de presión Renaware", FormInput::TEXT},
        {"Detalle", "detail", "Descripción detallada", FormInput::TEXTAREA},
        {"Precio", "unit_price", "Precio en colones", FormInput::TEXT},
        {"Cantidad", "stock", "Cantidad de artículos que posee", FormInput::TEXT}
    });

    if (!errors.empty()) {
        form.set_errors(errors);
    }

    content << form << std::endl;

    return content.str();
}

view::ProductAddBuilder::ProductAddBuilder(const std::string &title, const std::map<std::string, std::string> &errors)
        : PageBuilder(title), errors(errors) {}

