//
// Created by daniel on 16/10/19.
//

#include "PasswordHasher.h"
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <cassert>
#include <cstring>
#include "../util/log.h"

auth::PasswordHasher::PasswordHasher() {
    /* Load the human readable error strings for libcrypto */
    ERR_load_crypto_strings();
    /* Load all digest and cipher algorithms */
    OpenSSL_add_all_algorithms();
    /* Load config file, and other important initialisation */
    OPENSSL_config(NULL);

    digestAlg = EVP_sha256();
}

auth::PasswordHasher::~PasswordHasher() {

    /* Removes all digests and ciphers */
    EVP_cleanup();
    /* if you omit the next, a small leak may be left when you make use of the BIO (low level API) for e.g. base64 transformations */
    CRYPTO_cleanup_all_ex_data();
    /* Remove error strings */
    ERR_free_strings();

}

std::string auth::PasswordHasher::passwordHash(std::string username, std::string password) {

    unsigned char digest[KEY_LEN];

    unsigned char salt[SALT_BYTES];
    RAND_bytes(salt, SALT_BYTES);

    int work_factor = 0;
    float seconds = 0;

    while (seconds < 0.05 && work_factor < 1000) {
        clock_t t;

        ++work_factor;
        int iterations = work_factor*ITER_MIN;
        t = clock();
        int res = PKCS5_PBKDF2_HMAC(password.c_str(), password.length(),
                                    salt, SALT_BYTES, iterations,
                                    digestAlg,
                                    KEY_LEN, digest);
        if (res != 1) {
            // Manejar Error
        }

        t = clock() - t;
        seconds = ((float)t)/CLOCKS_PER_SEC;
    }

    int written;

    // Codificar
    int digestEncLen = (KEY_LEN/3 + (KEY_LEN%3 != 0))*4 + 1;
    unsigned char digestEnc[digestEncLen];
    written = EVP_EncodeBlock(digestEnc, digest, KEY_LEN);
    assert(written+1 == digestEncLen);

    int saltEncLen = (SALT_BYTES/3 + (SALT_BYTES%3 != 0))*4 + 1;
    unsigned char saltEnc[saltEncLen];
    written = EVP_EncodeBlock(saltEnc, salt, SALT_BYTES);
    assert(written + 1 == saltEncLen);

    int workEncLen = 3 + 1;
    char workEnc[workEncLen];
    sprintf(workEnc, "%03d", work_factor);

    // formato en BD: <work>$<digest>$<salt>
    int hashLen = strlen(workEnc) + strlen((char*)digestEnc) + strlen((char*)saltEnc) + 3;
    char hash[hashLen];
    sprintf(hash, "%s$%s$%s", workEnc, digestEnc, saltEnc);

    return std::string(hash);
}
