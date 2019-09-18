//
// Created by daniel on 16/09/19.
//

#include <sstream>
#include "ProductAddBuilder.h"
#include "../Form.h"

view::ProductAddBuilder::ProductAddBuilder(const std::string &title) : title(title) {}

std::string view::ProductAddBuilder::build_body() {
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

    body << view::Form("POST", "/product/add", {
            {"Título", "title", "p. ej. Olla de presión Renaware"},
            {"Detalle", "detail", "Descripción detallada"},
            {"Precio", "unit_price", "Precio en colones"},
            {"Cantidad", "stock", "Cnatidad de artículos que posee"}
    }) << std::endl;

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

