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

    std::string passwordHash(const std::string &password);
    bool passwordVerify(const std::string &password, const std::string &password_hash);

private:
    std::string hash_encode(int work_factor, unsigned char *digest, unsigned char *salt);
    void hash_decode(const std::string &encoded, int *work_factor, unsigned char *digest, unsigned char *salt);
    const EVP_MD *digestAlg;

    void handle_error();

};

}


#endif //APP_ECOMMERCE_PASSWORDHASHER_H
