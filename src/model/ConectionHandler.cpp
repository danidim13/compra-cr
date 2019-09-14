//
// Created by daniel on 14/09/19.
//

#include <mysql_driver.h>
#include "ConectionHandler.h"

model::ConnectionHandler::ConnectionHandler() {
    sql::Driver *driver = get_driver_instance();
    m_conn = std::unique_ptr<sql::Connection>(driver->connect(model::ConnOptions()));
}
