//
// Created by daniel on 16/09/19.
//

#include <sstream>
#include "UserLoginBuilder.h"
#include "../Form.h"

view::UserLoginBuilder::UserLoginBuilder(const std::string &title, const std::string &error) : PageBuilder(title),
                                                                                               error(error) {}

std::string view::UserLoginBuilder::build_content() {
    std::ostringstream body;
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

    return body.str();
}
