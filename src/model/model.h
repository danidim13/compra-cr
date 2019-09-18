//
// Created by daniel on 13/09/19.
//

#ifndef APP_ECOMMERCE_MODEL_H
#define APP_ECOMMERCE_MODEL_H

#include <string>
#include <list>
#include <vector>
#include <cppconn/resultset.h>
#include "mysql_driver.h"


namespace model{

    enum AttrType {
        INT,
        UINT,
        STRING,
        NONE
    };

    union AttrValue {
        int i;
        unsigned int u;
        const char *str;
    };

    class EntityAttr {
    public:
        EntityAttr();
        EntityAttr(int v);
        EntityAttr(unsigned int v);
        EntityAttr(std::string v);
        AttrType attrType;
        AttrValue attrValue;
        std::string attrStr;
//        bool empty;
    };

    class Entity {
    public:
        Entity() {};
        virtual ~Entity() {};
        virtual bool set_from_row(sql::ResultSet *res);
        virtual bool set_from_row(sql::ResultSet *res, const std::vector<std::string> &select);
        virtual bool set_from_map(const std::map<std::string, std::string> &map);
        virtual bool insert_autoId();
        virtual bool insert_autoId(const std::vector<std::string> &select);
        virtual std::vector<std::string> vector(const std::vector<std::string> &select) const;

    protected:
        std::map<std::string, EntityAttr> m_cols;
        std::string table;
    };
}

#endif //APP_ECOMMERCE_MODEL_H
