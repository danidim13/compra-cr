//
// Created by daniel on 13/09/19.
//

#include <sstream>
#include "model.h"
#include "ConnectionHandler.h"
#include <cstdlib>

model::EntityAttr::EntityAttr() {
    attrType = model::NONE;
}

model::EntityAttr::EntityAttr(int v) {
    attrType = model::INT;
    attrValue.i = v;
}

model::EntityAttr::EntityAttr(unsigned int v) {
    attrType = model::UINT;
    attrValue.u = v;
}

model::EntityAttr::EntityAttr(std::string v) {
    attrType = model::STRING;
    attrStr = v;
    attrValue.str = v.c_str();
}

bool model::Entity::set_from_row(sql::ResultSet *res)
{
    try {
        for (auto it = m_cols.begin(); it != m_cols.end(); ++it) {
            switch (it->second.attrType) {
                case model::INT: {
                    it->second.attrValue.i = res->getInt(it->first);
                    break;
                }
                case model::UINT: {
                    it->second.attrValue.u = res->getUInt(it->first);
                    break;
                }
                case model::STRING: {
                    it->second.attrStr = res->getString(it->first);
                    it->second.attrValue.str = (char*)it->second.attrStr.c_str();
                    break;
                }
                default:
                    break;
            }
        }
        return true;
    }
    catch (sql::SQLException e) {
        std::ostringstream error;
        error << "SQL ERROR (" << e.getErrorCode() << "): " << e.what();
        error << " on function "  << __FUNCTION__ << ", file " __FILE__ << ", line " << __LINE__;
        std::cerr << error.str();
        return false;
    }
}

bool model::Entity::set_from_row(sql::ResultSet *res, const std::vector<std::string> &select)
{
    try {
        for (auto attrName: select) {
            auto it = m_cols.find(attrName);
            if (it != m_cols.end()) {
                switch (it->second.attrType) {
                    case model::INT: {
                        it->second.attrValue.i = res->getInt(attrName);
                        break;
                    }
                    case model::UINT: {
                        it->second.attrValue.u = res->getUInt(attrName);
                        break;
                    }
                    case model::STRING: {
                        it->second.attrStr = res->getString(attrName);
                        it->second.attrValue.str = (char*)it->second.attrStr.c_str();
                        break;
                    }
                    default:
                        break;
                }
            }
        }
        return true;
    }
    catch (sql::SQLException e) {
        std::ostringstream error;
        error << "SQL ERROR (" << e.getErrorCode() << "): " << e.what();
        error << " on function "  << __FUNCTION__ << ", file " __FILE__ << ", line " << __LINE__;
        std::cerr << error.str();
        return false;
    }

}

bool model::Entity::set_from_map(const std::map<std::string, std::string> &map) {

    for (auto pair: map) {
        auto it = m_cols.find(pair.first);
        if (it != m_cols.end()) {
            switch (it->second.attrType) {
                case model::INT: {
                    it->second.attrValue.i = atoi(pair.second.c_str());
                    break;
                }
                case model::UINT: {
                    it->second.attrValue.u = strtoul(pair.second.c_str(), NULL, 10);
                    break;
                }
                case model::STRING: {
                    it->second.attrStr = pair.second;
                    it->second.attrValue.str = (char*)it->second.attrStr.c_str();
                    break;
                }
                default:
                    break;
            }
        }
    }
    return false;
}

bool model::Entity::insert_autoId(const std::vector<std::string> &select) {
    assert(!table.empty());

    std::ostringstream query;
    std::ostringstream columns;
    std::ostringstream values;

    columns << "(";
    values << "(";
    size_t len = select.size();

    for (auto col: select) {
        --len;
        if (col.compare("id") == 0) {
            continue;
        }
        columns << col;
        switch (m_cols[col].attrType) {
            case model::INT: {
                values << m_cols[col].attrValue.i;
                break;
            }
            case model::UINT: {
                values << m_cols[col].attrValue.u;
                break;
            }
            case model::STRING: {
                values << "'" << m_cols[col].attrStr << "'";
                break;
            }
            default:
                break;
        }
        if (len > 0) {
            columns << ", ";
            values << ", ";
        }
    }
    columns << ")" ;
    values << ")";
    query << "INSERT INTO " << table << " " << columns.str() << " VALUES " << values.str();

    return model::ConnectionHandler::execute(query.str());
}
bool model::Entity::insert_autoId() {

    assert(!table.empty());

    std::ostringstream query;
    std::ostringstream select;
    std::ostringstream values;

    select << "(";
    values << "(";
    size_t len = m_cols.size();

    for (auto pair: m_cols) {
        --len;
        if (pair.first.compare("id") == 0) {
            continue;
        }
        select << pair.first;
        switch (pair.second.attrType) {
            case model::INT: {
                values << pair.second.attrValue.i;
                break;
            }
            case model::UINT: {
                values << pair.second.attrValue.u;
                break;
            }
            case model::STRING: {
                values << "'" << pair.second.attrStr << "'";
                break;
            }
            default:
                break;
        }
        if (len > 0) {
            select << ", ";
            values << ", ";
        }
    }
    select << ")" ;
    values << ")";
    query << "INSERT INTO " << table << " " << select.str() << " VALUES " << values.str();

    return model::ConnectionHandler::execute(query.str());
}

std::vector<std::string> model::Entity::vector(const std::vector<std::string> &select) const {
    std::vector<std::string> entity;
    for (auto attrName: select) {
        auto it = m_cols.find(attrName);
        switch (it->second.attrType) {
            case INT: {
                entity.push_back(std::to_string(it->second.attrValue.i));
                break;
            }
            case UINT: {
                entity.push_back(std::to_string(it->second.attrValue.u));
                break;
            }
            case STRING: {
                entity.push_back(it->second.attrStr);
                break;
            }
        }
    }
    return entity;
}


/*

bool model::Entity::set_from_row(sql::ResultSet *res, std::vector<std::string> select)
{
    try {
        for (auto attrName: select) {
            auto it = m_cols.find(attrName);
            if (it != m_cols.end()) {
                switch (it->second.attrType) {
                    case model::INT: {
                        it->second.attrValue.i = res->getInt(attrName);
                        break;
                    }
                    case model::UINT: {
                        it->second.attrValue.u = res->getUInt(attrName);
                        break;
                    }
                    case model::STRING: {
                        it->second.attrStr = res->getString(attrName);
                        it->second.attrValue.str = it->second.attrStr.c_str();
                        break;
                    }
                    default:
                        break;
                }
            }
        }
        return true;
    }
    catch (sql::SQLException e) {
        std::ostringstream error;
        error << "SQL ERROR (" << e.getErrorCode() << "): " << e.what();
        error << " on function "  << __FUNCTION__ << ", file " __FILE__ << ", line " << __LINE__;
        std::cerr << error.str();
        return false;
    }
}

bool model::Entity::set_from_row(sql::ResultSet *res, std::set<std::string> select)
{
    try {
        for (auto attrName: select) {
            auto it = m_cols.find(attrName);
            if (it != m_cols.end()) {
                switch (it->second.attrType) {
                    case model::INT: {
                        it->second.attrValue.i = res->getInt(attrName);
                        break;
                    }
                    case model::UINT: {
                        it->second.attrValue.u = res->getUInt(attrName);
                        break;
                    }
                    case model::STRING: {
                        it->second.attrStr = res->getString(attrName);
                        it->second.attrValue.str = it->second.attrStr.c_str();
                        break;
                    }
                    default:
                        break;
                }
            }
        }
        return true;
    }
    catch (sql::SQLException e) {
        std::ostringstream error;
        error << "SQL ERROR (" << e.getErrorCode() << "): " << e.what();
        error << " on function "  << __FUNCTION__ << ", file " __FILE__ << ", line " << __LINE__;
        std::cerr << error.str();
        return false;
    }
}
 
*/
