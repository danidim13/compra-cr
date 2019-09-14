//
// Created by daniel on 13/09/19.
//

#include <sstream>
#include "model.h"

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

bool model::Entity::set_from_row(sql::ResultSet *res, std::list<std::string> select)
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
