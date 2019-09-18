//
// Created by daniel on 17/09/19.
//

#include <sstream>
#include "Table.h"

view::Table::Table(const std::vector<std::string> &header, const std::vector<std::vector<std::string>> &content) : header(
        header), content(content) {}

std::string view::Table::to_string() const {
    std::ostringstream table;
    table << R"(
<table cellpadding="0" cellspacing="0" class="table">
    <thead class="bg-light">
        <tr>
)";
    for (auto item: header) {
        table << R"(<th scope="col">)" << item << R"(</th>)" << std::endl;
    }
    table << R"(
        </tr>
    </thead>
    <tbody>
)";
    for (size_t row = 0; row < content.size(); ++row) {
        table << R"(
        <tr>)" << std::endl;

        for (size_t col = 0; col < content[row].size(); ++col) {
            table << R"(
            <td>)" << content[row][col] << "</td>" << std::endl;
        }
        table << R"(
        </tr>)" << std::endl;
    }
    table << R"(
    </tbody>
</table>
)";
    return table.str();
}

view::Table::Table(const std::vector<std::string> &header) : header(header) {}

std::ostream &view::operator<<(std::ostream &os, const view::Table &table) {
    os << table.to_string();
    return os;
}
