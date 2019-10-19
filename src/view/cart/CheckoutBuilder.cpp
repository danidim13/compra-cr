//
// Created by daniel on 17/09/19.
//

#include <sstream>
#include "CheckoutBuilder.h"
#include "../Form.h"

view::CheckoutBuilder::CheckoutBuilder(const std::string &title, const std::string &subtotal, const std::string &taxes,
                                       const std::string &total, const Table &cartTable, const std::string &error) : PageBuilder(title),
                                                                                                 subtotal(subtotal),
                                                                                                 taxes(taxes),
                                                                                                 total(total),
                                                                                                 cartTable(cartTable),
                                                                                                 error(error) {}

std::string view::CheckoutBuilder::build_content() {
    std::ostringstream body;
    view::Form form("POST", "/cart/checkout", "Comprar", {
        {"Nombre del tarjetahabiente", "card_holder", ""},
        {"Númbero de tarjeta", "card_number", "xxxx-xxxx-xxxx-xxxx"},
        {"Código de seguridad", "ccv", "xxx"},
        {"Fecha de vencimiento", "expiration_date", ""}
    });

    if (!formErrors.empty()) {
        form.set_errors(formErrors);
    }

    body << R"(
                    <div class="row">
                        <div class="col my-3">)";
    body << cartTable << R"(
                        </div>
                    </div>)";

    if (!error.empty()) {
        body << R"(
                    <div class="row">
                        <div class="col my-3">
                            <span class="text-danger">)" << error << R"(</span>
                        </div>
                    </div>)";
    }

    body << R"(
                    <div class="row my-3">
                        <div class="col-lg-3 col-sm-4">
                            <a class="btn btn-secondary btn-block" href="/cart/clear"/>Vaciar</a>
                        </div>
                    </div>
                    <div class="row">
                        <div class="col-lg-6 py-3">
    )";
    body << form << std::endl;

    body << R"(
                        </div>
                        <div class="col-lg-4 py-3">
                            <div class="row my-1">
                                <div class="col-6">
                                    <h3>Subtotal:</h3>
                                </div>
                                <div class="col">
                                    <p class="h4 d-flex justify-content-end">₡ )" << subtotal << R"(</p>
                                </div>
                            </div>
                            <div class="row my-1">
                                <div class="col-6">
                                    <h4>Impuestos:</h4>
                                </div>
                                <div class="col d-flex justify-content-end">
                                    <p class="h4">₡ )" << taxes << R"(</p>
                                </div>
                            </div>
                            <div class="row my-3">
                                <div class="col-6">
                                    <h2>Total:</h2>
                                </div>
                                <div class="col d-flex justify-content-end">
                                    <p class="h4">₡ )" << total << R"(</p>
                                </div>
                            </div>
                        </div>
                    </div>
                    <div class="my-5"></div>
)";

    return body.str();
}

view::CheckoutBuilder::CheckoutBuilder(const std::string &title, const std::string &subtotal,
                                       const std::string &taxes, const std::string &total,
                                       const view::Table &cartTable, const std::string &error,
                                       const std::map<std::string, std::string> &formErrors) : PageBuilder(title),
                                                                                              subtotal(subtotal),
                                                                                              taxes(taxes),
                                                                                              total(total),
                                                                                              cartTable(cartTable),
                                                                                              error(error),
                                                                                              formErrors(formErrors) {}
