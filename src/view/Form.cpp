//
// Created by daniel on 15/09/19.
//

#include <sstream>
#include "Form.h"

view::Form::Form(const std::string &method, const std::string &action, const std::string &submitLabel,
                 const std::vector<view::FormInput> &inputs) : method(method), action(action),
                                                               submit_label(submitLabel), inputs(inputs) {}

view::Form::Form(const std::string &method, const std::string &action, const std::vector<view::FormInput> &inputs)
        : method(method), action(action), inputs(inputs), submit_label("Submit") {}

view::Form::Form(const std::string &method, const std::string &action) : method(method), action(action),
                                                                         submit_label("Submit") {}

std::string view::Form::to_string() const {
    std::ostringstream form;

    form << R"(
    <div class="row">
        <div class="col">
            <form action=")" << action << R"(" method=")" << method <<R"(">)" << std::endl;
    for (auto input: inputs) {
        form << input;
    }
    form << R"(
                <button type="submit" class="btn btn-primary col-2">Submit</button>
            </form>
        </div>
    </div>)" << std::endl;


    return form.str();
}

std::ostream &view::operator<<(std::ostream &os, const view::Form &form) {
    return os << form.to_string();
}
