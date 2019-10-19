//
// Created by daniel on 15/09/19.
//

#include <sstream>
#include "UserAddBuilder.h"
#include "../Form.h"

view::UserAddBuilder::UserAddBuilder(const std::string &title) : PageBuilder(title) {}

std::string view::UserAddBuilder::build_content() {
    std::ostringstream content;

    view::Form form("POST", "/user/add", {
            {"Nombre de usuario", "username", "ejemplo"},
            {"Contraseña", "password", "Debe tener al menos 8 caracteres, mayúscula, minúscula y un número o caracter especial"},
            {"Nombre", "surname", "Nombre"},
            {"Apellido(s)", "last_name", "Apellidos"},
            {"Correo electrónico", "email", "ejemplo@gmail.com"},
            {"Teléfono", "phone", "8080-2020"}
    });

    if (!errors.empty()) {
        form.set_errors(errors);
    }

    content << form << std::endl;

    return content.str();
}

view::UserAddBuilder::UserAddBuilder(const std::string &title, const std::map<std::string, std::string> &errors)
        : PageBuilder(title), errors(errors) {}
