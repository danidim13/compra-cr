//
// Created by daniel on 10/09/19.
//

#include "view/DomItem.h"
#include "view/NavItem.h"
#include "view/PageBuilder.h"
#include <iostream>
#include <mysql_driver.h>
#include <cppconn/exception.h>

void test_sql() {
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *conn;
    driver = sql::mysql::get_mysql_driver_instance();

    try {
        conn = driver->connect("localhost", "ecommerce_app", "seguridad2019");
    } catch (sql::SQLException e) {

        std::cout << "Conexión fallida: " << "[" << e.getErrorCode() << "] " << e.what() << std::endl;
        exit(1);
    }


    if (conn->isValid()) {
        std::cout << "Conexión exitosa!" << std::endl;
    } else {
        std::cout << "Conexión fallida :(" << std::endl;
    }

    delete conn;
//    sql::ConnectOptionsMap()
}

void test_dom() {
    DomItem item1("a",
            {
                {"class", "nav-item nav-link"},
                {"href", "#"}
            },
            DomItem::SIMPLE, nullptr);

    DomItem item2("input",
                  {
                          {"class", "form-input"},
                          {"type", "password"}
                  },
                  DomItem::OPEN, nullptr);

    NavItem item3("local",
            "Inicio",
            {
                {"class", "nav-item nav-link"},
            });


    std::cout << item1 << std::endl << item2 << std::endl << item3 << std::endl;

}

void test_builder() {
    PageBuilder pageBuilder;
    std::cout << pageBuilder.build_document() << std::endl;
}

int main(int argc, char *argv[]) {

    test_sql();
//    test_dom();
//    test_builder();

}

