//
// Created by daniel on 14/09/19.
//

#ifndef APP_ECOMMERCE_CONECTIONHANDLER_H
#define APP_ECOMMERCE_CONECTIONHANDLER_H


#include <vector>

#include <mysql_driver.h>
#include <cppconn/connection.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#include <sstream>
#include "model.h"
#include "../util/log.h"

namespace model {

inline sql::ConnectOptionsMap &ConnOptions() {
    static sql::ConnectOptionsMap connOpt{
            {"hostName", "localhost"},
            {"userName", "ecommerce_app"},
            {"password", "seguridad2019"},
            {"schema", "ecommerce"}
    };
}

class ConnectionHandler {
public:

    template <class T>
    static std::vector<T> executeQuery(const std::string &query) {
        static_assert(std::is_base_of<Entity, T>::value, "T is not derived from Entity");

        sql::mysql::MySQL_Driver *driver;
//        sql::ConnectOptionsMap options = model::ConnOptions();
        std::unique_ptr<sql::Connection> conn;
        std::unique_ptr<sql::Statement> stmt;
        std::unique_ptr<sql::ResultSet> res;

        std::vector<T> entities;

        try {
            driver = sql::mysql::get_mysql_driver_instance();
//            conn = std::unique_ptr<sql::Connection>(driver->connect(model::ConnOptions()));

            conn = std::unique_ptr<sql::Connection>(driver->connect("localhost", "ecommerce_app", "seguridad2019"));
            conn->setSchema("ecommerce");

            stmt = std::unique_ptr<sql::Statement>(conn->createStatement());
            res = std::unique_ptr<sql::ResultSet>(stmt->executeQuery(query));
            while(res->next()) {
                T entity;
                entity.set_from_row(res.get());
                entities.push_back(entity);
            }

            return entities;
        } catch (sql::SQLException e) {
            std::ostringstream error_msg;
            error_msg << "SQL Error (" << e.getErrorCode() << ") " << e.what()
                      << " while executing '" << query
                      << "' on function " << __FUNCTION__ << ", file " __FILE__
                      << ", line " << __LINE__  << std::endl;

            std::cerr << error_msg.str();
            log_error(NULL, (char*)(error_msg.str().c_str()));

            entities.clear();
            return entities;
        }

    };

    template <class T>
    static std::vector<T> executeQuery(const std::string &query, const std::list<std::string> &select) {
        static_assert(std::is_base_of<Entity, T>::value, "T is not derived from Entity");

        sql::mysql::MySQL_Driver *driver;
        std::unique_ptr<sql::Connection> conn;
        std::unique_ptr<sql::Statement> stmt;
        std::unique_ptr<sql::ResultSet> res;

        std::vector<T> entities;

        try {
            driver = sql::mysql::get_mysql_driver_instance();
            conn = std::unique_ptr<sql::Connection>(driver->connect(model::ConnOptions()));

            stmt = std::unique_ptr<sql::Statement>(conn->createStatement());
            res = std::unique_ptr<sql::ResultSet>(stmt->executeQuery(query));
            while(res->next()) {
                T entity;
                entity.set_from_row(res.get(), select);
                entities.push_back(entity);
            }

            return entities;
        } catch (sql::SQLException e) {
            std::ostringstream error_msg;
            error_msg << "SQL Error (" << e.getErrorCode() << ") " << e.what()
                      << " while executing '" << query
                      << "' on function " << __FUNCTION__ << ", file " __FILE__
                      << ", line " << __LINE__  << std::endl;

            std::cerr << error_msg.str();
            log_error(NULL, (char*)(error_msg.str().c_str()));

            entities.clear();
            return entities;
        }

    };
};

}

#endif //APP_ECOMMERCE_CONECTIONHANDLER_H
