//
// Created by daniel on 15/09/19.
//

#include <sstream>
#include "FormInput.h"

view::FormInput::FormInput(const std::string &label, const std::string &name, const std::string &placeholder) : label(
        label), name(name), placeholder(placeholder) {}

view::FormInput::FormInput(const std::string &label, const std::string &name) : label(label), name(name), placeholder("") {}

std::string view::FormInput::to_string() const {
    std::ostringstream html;
    html << R"(
    <div class="form-group">
        <label>)" << label << R"(</label>
        <input type="text" class="form-control" name=")" << name << R"(" placeholder=")" << placeholder << R"(">
    </div>
)";
    return html.str();
}

std::ostream &view::operator<<(std::ostream &os, const view::FormInput &input) {
    return os << input.to_string();
}
