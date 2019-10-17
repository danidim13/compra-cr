//
// Created by daniel on 17/09/19.
//

#ifndef APP_ECOMMERCE_TABLE_H
#define APP_ECOMMERCE_TABLE_H


#include <string>
#include <vector>
#include <ostream>

namespace view {

class Table {
public:
    Table() = default;
    Table(const std::vector<std::string> &header);
    Table(const std::vector<std::string> &header, const std::vector<std::vector<std::string>> &content);

    std::vector<std::string> header;
    std::vector<std::vector<std::string>> content;

    std::string to_string() const;

    friend std::ostream &operator<<(std::ostream &os, const Table &table);
};
std::ostream &operator<<(std::ostream &os, const Table &table);

}

#endif //APP_ECOMMERCE_TABLE_H
