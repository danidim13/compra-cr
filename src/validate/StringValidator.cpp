//
// Created by daniel on 15/10/19.
//

#include "StringValidator.h"

validate::StringValidator::StringValidator(const std::regex &format, int minLength,
                                            int maxLength) : format(format), minLength( minLength),
                                            maxLength( maxLength) {}

std::pair<bool, std::string> validate::StringValidator::validate(const std::string &src) {
    if (!length_bewteen(src, minLength, maxLength)) {
        if (src.length() == 0)
            return {false, "Campo requerido"};

        std::stringstream msg;
        msg << "Debe tener al menos " << minLength << " y máximo " << maxLength << " caracteres";
        return {false, msg.str()};
    }
    if (src.length() > 0 && !regex(src, format)) {
        return {false, "Formato inválido"};
    }
    return {true, ""};
}
