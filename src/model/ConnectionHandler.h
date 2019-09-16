//
// Created by daniel on 14/09/19.
//

#ifndef APP_ECOMMERCE_CONNECTIONHANDLER_H
#define APP_ECOMMERCE_CONNECTIONHANDLER_H


#include <vector>

#include <mysql_driver.h>
#include <cppconn/connection.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#include <sstream>
#include "model.h"
#include "../util/log.h"

#define DB_HOST "localhost"
#define DB_USER "ecommerce_app"
#define DB_PASS "seguridad2019"
#define DB_INST "ecommerce"

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
    static std::vector<T> executeQuery(const std::string &query, const std::list<std::string> &select, const bool &useSel = true) {
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

            conn = std::unique_ptr<sql::Connection>(driver->connect(DB_HOST, DB_USER, DB_PASS));
            conn->setSchema(DB_INST);

            stmt = std::unique_ptr<sql::Statement>(conn->createStatement());


            log_info(NULL, (char*) std::string("Ejecutando query: ").append(query).c_str());

            res = std::unique_ptr<sql::ResultSet>(stmt->executeQuery(query));

            while(res->next()) {
                T entity;
                if (useSel) {
                    entity.set_from_row(res.get(), select);
                } else {
                    entity.set_from_row(res.get());
                }
                entities.push_back(entity);
            }

            std::ostringstream msg;
            msg << "Se obtuvieron " << entities.size() << " tuplas";
            log_info(NULL, (char*) msg.str().c_str());

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
    static std::vector<T> executeQuery(const std::string &query) {
        return executeQuery<T>(query, {}, false);
    };

    static bool execute(const std::string &query) {
        sql::mysql::MySQL_Driver *driver;
        std::unique_ptr<sql::Connection> conn;
        std::unique_ptr<sql::Statement> stmt;

        try {
            driver = sql::mysql::get_mysql_driver_instance();

            conn = std::unique_ptr<sql::Connection>(driver->connect(DB_HOST, DB_USER, DB_PASS));
            conn->setSchema(DB_INST);

            stmt = std::unique_ptr<sql::Statement>(conn->createStatement());
            stmt->execute(query);

            return true;
        } catch (sql::SQLException e) {
            std::ostringstream error_msg;
            error_msg << "SQL Error (" << e.getErrorCode() << ") " << e.what()
                      << " while executing '" << query
                      << "' on function " << __FUNCTION__ << ", file " __FILE__
                      << ", line " << __LINE__  << std::endl;

            std::cerr << error_msg.str();
            log_error(NULL, (char*)(error_msg.str().c_str()));

            return false;
        }

    }

};

}

#endif //APP_ECOMMERCE_CONNECTIONHANDLER_H
