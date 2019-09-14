//
// Created by daniel on 14/09/19.
//

#ifndef APP_ECOMMERCE_CONECTIONHANDLER_H
#define APP_ECOMMERCE_CONECTIONHANDLER_H


#include <cppconn/connection.h>

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
        ConnectionHandler();

    protected:
        std::unique_ptr<sql::Connection> m_conn;

    };
}

#endif //APP_ECOMMERCE_CONECTIONHANDLER_H
