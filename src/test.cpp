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
#include <ctime>
#include "util/cookie.h"
#include "model/CardPayment.h"
#include "auth/PasswordHasher.h"
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include "nlohmann/json.hpp"

void json_test() {
    using json = nlohmann::json;
    json j = R"(
    {
      "shopping_cart" : [1, 2, 1, 5],
      "errors" : {
        "f1" : "val1",
        "f2" : "val2"
      }
    }
)"_json;

    json::iterator it = j.find("shopping_cart");
    if (it != j.end()) {
        assert(it.value().is_array());
        std::vector<int> v = it.value().get<std::vector<int>>();
        for (auto datum: v) {
            std::cout << datum << std::endl;
        }
    }

    /*
    std::cout << "Valor serializado" << std::endl;
    std::cout << j << std::endl;


    std::cout << "Iterar sobre el array" << std::endl;
    for (json::iterator it = j.begin(); it != j.end(); ++it) {
        std::cout << it.key() << " : ";
        if (it.value().size() > 1) {
             std::cout << std::endl;
             for (json::iterator it2 = it.value().begin(); it2 != it.value().end(); ++it2) {
                 std::cout << it.value() << std::endl;
             }
        } else {
            std::cout << it.value() << std::endl;
        }
    }
     */
}

void encode_test() {

    unsigned char data[8] = {1, 2, 4, 8, 16, 32, 64, 128};
    unsigned char enco[9];

    for (int i = 1; i < 8; i++) {
        EVP_EncodeBlock(enco, data, i);
        std::cout << std::string((char*)enco) << std::endl;
    }
}

void passwd_test() {
    auth::PasswordHasher hasher;
    auto res = hasher.passwordHash("contraseña");
    std::cout << res << std::endl;

    if (hasher.passwordVerify("contraseña", res)) {
        std::cout << "contraseña matches password" << std::endl;
    }

    if (!hasher.passwordVerify("blabla", res)) {
        std::cout << "blabla doesnt match password" << std::endl;
    }

    res = hasher.passwordHash("Juan1234");
    std::cout << "Contraseña Juan1234: " << res << std::endl;
    res = hasher.passwordHash("Mari1234");
    std::cout << "Contraseña Mari1234: " << res << std::endl;

}

void crypt_test() {
    EVP_MD_CTX *mdctx;
    const EVP_MD *digest;
    char mess1[] = "Test Message\n";
    char mess2[] = "Hello World\n";
    unsigned char md_value[EVP_MAX_MD_SIZE];
    unsigned int md_len, i;

    int is = 64;
    int enc_max_size = 4*(is + (3 - is%3))/3 + 1;

    unsigned char enc_md_value[enc_max_size];
    unsigned int enc_md_len;

    digest = EVP_sha256();

    int iter = 1000;
    int saltlen = 8;
    int keylen = 32;

    unsigned char salt[saltlen];
    RAND_bytes(salt, saltlen);

    PKCS5_PBKDF2_HMAC(mess1, strlen(mess1),
                          salt, saltlen, iter,
                          digest,
                          keylen, md_value);

    /*
    mdctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(mdctx, md, NULL);
    EVP_DigestUpdate(mdctx, mess1, strlen(mess1));
    EVP_DigestUpdate(mdctx, mess2, strlen(mess2));
    EVP_DigestFinal_ex(mdctx, md_value, &md_len);
    EVP_MD_CTX_free(mdctx);


     */

    enc_md_len = EVP_EncodeBlock(enc_md_value, md_value, keylen);

    printf("Digest (%d-bit) is: ", keylen);
    for (i = 0; i < keylen; i++)
        printf("%02x", md_value[i]);
    printf("\n");

    printf("Digest (b64) is: %s\n", enc_md_value);

    exit(0);
}

void payment_test() {
    auto result = model::CardPayment::process("5000.00", {
        {"card_holder", "DANIEL DIAZ"},
        {"card_number", "4042-2000-6969"},
//        {"card_number", "40"},
        {"ccv", "555"},
        {"expiration_date", "5/55"}});
    std::cout << result.first << " " << result.second << std::endl;
}

void test_cart() {
    auto itemMap = split_cart_str("11,4,5,7,11,1");
    for (auto item: itemMap) {
        std::cout << "ID: " << item.first <<", cantidad: " << item.second << std::endl;
    }
}

void test_time() {
    time_t now;
    time(&now);
    std::cout << "Now:    " << format_cookie_time(now) << std::endl;
    std::cout << "Renew:  " << renewed_time() << std::endl;
    std::cout << "Expire: " << expired_time() << std::endl;

}

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

    json_test();
//    encode_test();
//    passwd_test();
//    crypt_test();
//    payment_test();
//    test_cart();
//    test_time();
//    test_decode();
//    test_latest_products();
//    test_sql();
//    test_dom();
//    test_builder();

}

