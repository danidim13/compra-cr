//
// Created by daniel on 15/09/19.
//

#include <sstream>
#include "FormInput.h"

view::FormInput::FormInput(const std::string &label, const std::string &name) :
        label(label), name(name), placeholder(""), type(view::FormInput::PASSWORD) {}

view::FormInput::FormInput(const std::string &label, const std::string &name, const std::string &placeholder,
                           const Type &type) : label(
        label), name(name), placeholder(placeholder), type(type) {}

view::FormInput::FormInput(const std::string &label, const std::string &name, const Type &type)
        : label(label), name(name), placeholder(""), type(type) {}

view::FormInput::FormInput(const std::string &label, const std::string &name, const std::string &placeholder,
                           const std::string &error, const Type &type) : label(label), name(name), placeholder(placeholder),
                                                       error(error), type(type) {}

std::string view::FormInput::to_string() const {
    switch (type) {
        case TEXT: return str_text();
        case PASSWORD: return str_password();
        case TEXTAREA: return str_textarea();
    }
}
std::string view::FormInput::str_text() const {
    std::ostringstream html;
    html << R"(
    <div class="form-group">
        <label>)" << label << R"(</label>
        <input type="text" autocomplete="off" class="form-control)";

    if (!error.empty()) html << " is-invalid";

    html << R"(" name=")" << name << R"(" placeholder=")" << placeholder << R"(">)";

    if(!error.empty()) {
        html << R"(
        <div class="invalid-feedback">
            )" << error << R"(
        </div>
)";
    }

    html << R"(
    </div>
)";
    return html.str();
}

std::string view::FormInput::str_textarea() const {
    std::ostringstream html;
    html << R"(
    <div class="form-group">
        <label>)" << label << R"(</label>
        <textarea type="text" autocomplete="off" class="form-control)";

    if (!error.empty()) html << " is-invalid";

    html << R"(" name=")" << name << R"(" placeholder=")" << placeholder << R"(" rows="3"></textarea>)";

    if(!error.empty()) {
        html << R"(
        <div class="invalid-feedback">
            )" << error << R"(
        </div>
)";
    }

    html << R"(
    </div>
)";
    return html.str();
}

std::string view::FormInput::str_password() const {
    std::ostringstream html;
    html << R"(
    <div class="form-group">
        <label>)" << label << R"(</label>
        <input type="password" autocomplete="off" class="form-control)";

    if (!error.empty()) html << " is-invalid";

    html << R"(" name=")" << name << R"(" placeholder=")" << placeholder << R"(">)";

    if(!error.empty()) {
        html << R"(
        <div class="invalid-feedback">
            )" << error << R"(
        </div>
)";
    }

    html << R"(
    </div>
)";
    return html.str();
}
std::ostream &view::operator<<(std::ostream &os, const view::FormInput &input) {
    return os << input.to_string();
}
