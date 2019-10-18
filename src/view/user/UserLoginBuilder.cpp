//
// Created by daniel on 16/09/19.
//

#include <sstream>
#include "UserLoginBuilder.h"
#include "../Form.h"

view::UserLoginBuilder::UserLoginBuilder(const std::string &title, const std::string &error) : title(title),
                                                                                               error(error) {}

std::string view::UserLoginBuilder::build_body() {
    std::ostringstream body;

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

    body << view::Form("POST", "/user/login", {
            {"Nombre de usuario", "username", "ejemplo"},
            {"Contraseña", "password", "********"}
    }) << std::endl;

    if (!error.empty()) {
        body << R"(
                    <div class="row">
                        <div class="col text-danger"
                            <p>)"
                            << error << R"(
                            </p>
                        </div>
                    </div>
)";
    }
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
