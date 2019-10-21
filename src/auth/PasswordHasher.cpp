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
#include <sstream>
#include "../util/log.h"

auth::PasswordHasher::PasswordHasher() {
    digestAlg = EVP_sha256();
}

auth::PasswordHasher::~PasswordHasher() {

}

std::string auth::PasswordHasher::passwordHash(const std::string &password) {

    unsigned char digest[KEY_LEN];

    unsigned char salt[SALT_BYTES];
    RAND_bytes(salt, SALT_BYTES);

    int work_factor = 0;
    float seconds = 0;

    while (seconds < CRYPT_TARGET_SEC && work_factor < 999) {
        clock_t t;

        ++work_factor;
        int iterations = work_factor*ITER_MIN;
        t = clock();
        int res = PKCS5_PBKDF2_HMAC(password.c_str(), password.length(),
                                    salt, SALT_BYTES, iterations,
                                    digestAlg,
                                    KEY_LEN, digest);
        if (res != 1) {
            handle_error(__FUNCTION__, __LINE__);
        }

        t = clock() - t;
        seconds = ((float)t)/CLOCKS_PER_SEC;
    }
    int written;
    return hash_encode(work_factor, digest, salt);
}

bool auth::PasswordHasher::passwordVerify(const std::string &password, const std::string &password_hash) {

    // formato en BD: <work>$<digest>$<salt>
    int work_factor;
    unsigned char digest[KEY_LEN];
    unsigned char salt[SALT_BYTES];

    hash_decode(password_hash, &work_factor, digest, salt);

    // Obtener el hash de la constraseña suministrada
    unsigned char digestCompare[KEY_LEN];

    int iterations = work_factor*ITER_MIN;
    int res = PKCS5_PBKDF2_HMAC(password.c_str(), password.length(),
                                salt, SALT_BYTES, iterations,
                                digestAlg,
                                KEY_LEN, digestCompare);
    if (res == 0) {
        handle_error(__FUNCTION__, __LINE__);
    }

    return memcmp(digest, digestCompare, KEY_LEN) == 0;
}

std::string auth::PasswordHasher::hash_encode(int work_factor, unsigned char *digest, unsigned char *salt) {

    int written;
    int digestEncLen = (KEY_LEN/3 + (KEY_LEN%3 != 0))*4 + 1;
    int saltEncLen = (SALT_BYTES/3 + (SALT_BYTES%3 != 0))*4 + 1;
    int workEncLen = 3 + 1;

    unsigned char digestEnc[digestEncLen];
    unsigned char saltEnc[saltEncLen];
    char workEnc[workEncLen];

    written = EVP_EncodeBlock(digestEnc, digest, KEY_LEN);
    if (written == -1) {
        handle_error(__FUNCTION__, __LINE__);
    }
    assert(written + 1 == digestEncLen);

    written = EVP_EncodeBlock(saltEnc, salt, SALT_BYTES);
    if (written == -1) {
        handle_error(__FUNCTION__, __LINE__);
    }
    assert(written + 1 == saltEncLen);

    sprintf(workEnc, "%03d", work_factor);

    // formato en BD: <work>$<digest>$<salt>
    int hashLen = strlen(workEnc) + strlen((char*)digestEnc) + strlen((char*)saltEnc) + 3;
    char hash[hashLen];
    sprintf(hash, "%s$%s$%s", workEnc, digestEnc, saltEnc);

    return std::string(hash);
}

void auth::PasswordHasher::hash_decode(const std::string &encoded, int *work_factor, unsigned char *digest,
                                       unsigned char *salt) {
    size_t workEncLen = 3 + 1;
    size_t digestEncLen = (KEY_LEN/3 + (KEY_LEN%3 != 0))*4 + 1;
    size_t saltEncLen = (SALT_BYTES/3 + (SALT_BYTES%3 != 0))*4 + 1;

    unsigned char saltEnc[saltEncLen];
    unsigned char digestEnc[digestEncLen];

    int hashLen = workEncLen + digestEncLen + saltEncLen;
    assert(encoded.length()+1 == hashLen);

    sscanf(encoded.c_str(), "%03d$%[^$]$%s", work_factor, (char*)digestEnc, (char*)saltEnc);

    saltEnc[saltEncLen] = '\0';
    digestEnc[digestEncLen] = '\0';

    // Decodificar digest y salt
    int decoded;

    decoded = EVP_DecodeBlock(digest, digestEnc, digestEncLen-1);
    if (decoded == -1) {
        handle_error(__FUNCTION__, __LINE__);
    }
    assert(decoded == KEY_LEN);

    decoded = EVP_DecodeBlock(salt, saltEnc, saltEncLen-1);
    if (decoded == -1) {
        handle_error(__FUNCTION__, __LINE__);
    }
    assert(decoded == SALT_BYTES);

}

void auth::PasswordHasher::handle_error(const std::string &f, const int &l) {
    std::ostringstream error_msg;
    unsigned long e = ERR_get_error();
    error_msg << "SSL Error (" << ERR_error_string(e, NULL) << ") "
              << "on function " << f << ", file " __FILE__
              << ", line " << l << std::endl;

    log_error(NULL, (char*)error_msg.str().c_str());
    exit(1);
}
