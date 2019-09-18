//
// Created by daniel on 17/09/19.
//

#include <sstream>
#include "CheckoutBuilder.h"

view::CheckoutBuilder::CheckoutBuilder(const std::string &title, const std::string &subtotal, const std::string &taxes,
                                       const std::string &total, const view::Table &cartTable) : title(title),
                                                                                                 subtotal(subtotal),
                                                                                                 taxes(taxes),
                                                                                                 total(total),
                                                                                                 cartTable(cartTable) {}

std::string view::CheckoutBuilder::build_body() {
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
                    </div>
                    <div class="row">
                        <div class="col my-3">)";
    body << cartTable << R"(
                        </div>
                    </div>
                    <div class="row">
                        <div class="col-4">
                            <h5>Subtotal:</h4>
                        </div>
                        <div class="col">
                            <p>₡ )" << subtotal << R"(</p>
                        </div>
                    </div>
                    <div class="row">
                        <div class="col-4">
                            <h6>Impuestos:</h4>
                        </div>
                        <div class="col">
                            <p>₡ )" << taxes << R"(</p>
                        </div>
                    </div>
                    <div class="row">
                        <div class="col-4">
                            <h4>Total:</h4>
                        </div>
                        <div class="col">
                            <p>₡ )" << total << R"(</p>
                        </div>
                    </div>
                    <div class="row my-4">
                        <div class="col-2">
                            <form action="/cart/checkout" method="post">
                                <button type="submit" class="btn btn-primary"/>Comprar</button>
                            </form>
                        </div>
                        <div class="col-2">
                            <a type="button" class="btn btn-secondary" href="/cart/clear"/>Vaciar</a>
                        </div>
                    </div>

)";
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
