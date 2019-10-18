//
// Created by daniel on 15/09/19.
//

#include <sstream>
#include "UserAddBuilder.h"
#include "../Form.h"

view::UserAddBuilder::UserAddBuilder(const std::string &title) : title(title) {}

std::string view::UserAddBuilder::build_body() {
    std::ostringstream body;

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

    body << R"(
<!-- Inicio contenido -->
        <main>
        <div class="container">
)";
     body << R"(
            <!-- Inicio Ofertas/últimos artículos/Resultado de búsqueda -->
            <div class="row bg-white page-title">
                <div class="col">
                    <div class="my-3"></div>
                    <div class="row">
                        <div class="col">
                            <h1 class="my-3" style="font-family: 'Alfa Slab One', cursive;">
)";
    body << title << R"(
                            </h1>
                        </div>
                    </div>)";

    body << form << std::endl;

    body << R"(
                    <div class="my-5"></div>
                </div>
            </div>
            <!-- Fin Ofertas/últimos artículos -->

        </div>
    </main>
    <!-- Fin contenido -->
)";

    return body.str();
}

view::UserAddBuilder::UserAddBuilder(const std::string &title, const std::map<std::string, std::string> &errors)
        : title(title), errors(errors) {}
