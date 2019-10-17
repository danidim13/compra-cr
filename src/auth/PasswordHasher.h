//
// Created by daniel on 16/10/19.
//

#ifndef APP_ECOMMERCE_PASSWORDHASHER_H
#define APP_ECOMMERCE_PASSWORDHASHER_H


#include <string>
#include <openssl/ossl_typ.h>

namespace auth {

    const int SALT_BYTES = 6;
    const int KEY_LEN = 48;
    const int ITER_MIN = 2000;

class PasswordHasher {
public:
    PasswordHasher();

    virtual ~PasswordHasher();

    std::string passwordHash(std::string username, std::string password);

private:
    const EVP_MD *digestAlg;

};

}


#endif //APP_ECOMMERCE_PASSWORDHASHER_H
