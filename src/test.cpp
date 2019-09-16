//
// Created by daniel on 10/09/19.
//

#include "view/DomItem.h"
#include "view/NavItem.h"
#include "view/PageBuilder.h"
#include <iostream>
#include <mysql_driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <vector>
#include "model/model.h"
#include "model/User.h"
#include "model/Product.h"
#include "util/decode.h"

void test_sql() {
    sql::mysql::MySQL_Driver *driver;
    std::unique_ptr<sql::Connection> conn;
    std::unique_ptr<sql::Statement> stmt;
    std::unique_ptr<sql::ResultSet> res;

    try {
        driver = sql::mysql::get_mysql_driver_instance();
        conn = std::unique_ptr<sql::Connection>(driver->connect("localhost", "ecommerce_app", "seguridad2019"));

        if (conn->isValid()) {
            std::cout << "Conexión exitosa!" << std::endl;
        } else {
            std::cout << "Conexión fallida :(" << std::endl;
        }

        conn->setSchema("ecommerce");

        stmt = std::unique_ptr<sql::Statement>(conn->createStatement());
        res = std::unique_ptr<sql::ResultSet>(stmt->executeQuery("SELECT * FROM users"));
        std::vector<model::User> users;
        while(res->next()) {
            model::User user;
//            user.set_from_row(res.get(), {"id", "username"});
            user.set_from_row(res.get());
            users.push_back(user);
        }

        for (auto user: users) {
            std::cout << "ID " << user.id();
            std::cout << ", USERNAME " << user.username();
            std::cout << std::endl;
        }

    } catch (sql::SQLException e) {
        std::cout << "SQL Error" << "[" << e.getErrorCode() << "] " << e.what() << std::endl;
        exit(1);
    }


//    sql::ConnectOptionsMap()
}

void test_latest_products() {
    std::vector<model::Product> latest( std::move(model::Product::getLatestN(9)) );

    if (!latest.empty()) {
        for (std::vector<model::Product>::iterator it = latest.begin(); it != latest.end(); ++it) {
            std::cout << "ID " << it->id();
            std::cout << ", TITLE " << it->title();
            std::cout << ", PRICE " << it->unit_price();
            std::cout << std::endl;
        }
    } else {
        std::cout << "No había ma'na" << std::endl;
    }
}

void test_dom() {
    view::DomItem item1("a",
            {
                {"class", "nav-item nav-link"},
                {"href", "#"}
            },
            view::DomItem::SIMPLE, nullptr);

    view::DomItem item2("input",
                  {
                          {"class", "form-input"},
                          {"type", "password"}
                  },
                  view::DomItem::OPEN, nullptr);

    view::NavItem item3("local",
            "Inicio",
            {
                {"class", "nav-item nav-link"},
            });


    std::cout << item1 << std::endl << item2 << std::endl << item3 << std::endl;

}

void test_decode() {

    char str1[] = "search=canci%C3%B3n";
    char str2[] = "username=danidim13&surname=Daniel&last_name=D%C3%ADaz+Molina&email=daniel.correo%40gmail.com&phone=8707-8691";
    char str3[] = "username=danidim13&surname=Daniel&last_name=D%C3%ADaz+Molina&email=daniel.correo%40gmail.com&phone=8707-8691";
    std::string str1_dec(url_decode(str1));
    std::cout << str1_dec << std::endl;
    std::cout << url_decode(str2) << std::endl;

    url_decode(str3, str3);
    printf("%s\n", str3);

    std::map<std::string, std::string> qMap = split_query(str2);
    for (auto pair: qMap) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

}

void test_builder() {
    view::PageBuilder pageBuilder;
    std::cout << pageBuilder.build_document() << std::endl;
}

int main(int argc, char *argv[]) {

    test_decode();
//    test_latest_products();
//    test_sql();
//    test_dom();
//    test_builder();

}

