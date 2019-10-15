//
// Created by daniel on 15/10/19.
//

#include <vector>
#include "EmailValidator.h"

validate::EmailValidator::EmailValidator() {}

std::pair<bool, std::string> validate::EmailValidator::validate(const std::string &src) {

    if (!length_bewteen(src, 1, 256)) {
        return {false, "Longitud máxima 256 caracteres"};
    }
    if (!regex(src, REGEX_EMAIL)) {
        return {false, "Error de formato, debe ser un correo"};
    }
    return {true, ""};
}
