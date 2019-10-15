//
// Created by daniel on 15/10/19.
//

#include "MapValidator.h"

validate::MapValidator::MapValidator(const std::map<std::string, validate::MapValidator::ptrValidator_t> &fields)
        : fields(fields) {}

validate::MapValidatorResult validate::MapValidator::validate(const std::map<std::string, std::string> &raw_data) {
    MapValidatorResult result;
    result.valid = true;

    auto data_it = raw_data.begin();
    auto field_it = fields.begin();

    while (data_it != raw_data.end() || field_it != fields.end()) {

        int comp = data_it->first.compare(field_it->first);
        if ( comp < 0) {
            // Dato extra
            result.valid = false;
            result.errors[data_it->first] = "Invalid field";
            ++data_it;
        } else if (comp == 0) {
            auto fieldResult = field_it->second->validate(data_it->second);
            if (!fieldResult.first) {
                result.valid = false;
                result.errors[data_it->first] = fieldResult.second;
            }
            ++data_it;
            ++field_it;
        } else {
            // Dato faltante
            result.valid = false;
            result.errors[field_it->first] = "Dato faltante";
            ++field_it;
        }
    }

    return result;
}
