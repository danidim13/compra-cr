//
// Created by daniel on 17/10/19.
//

#include "SessionManager.h"
#include "../util/log.h"
#include <openssl/conf.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <cassert>
#include <sstream>

std::string auth::SessionManager::genSID() {

    assert(SID_LEN%4 == 0);

    int r;
    int byteslen = SID_LEN/4*3;
    unsigned char bytes[byteslen];
    unsigned char sid[SID_LEN+1];

    r = RAND_bytes(bytes, byteslen);
    if (r != 1) {
        handle_error(__FUNCTION__, __LINE__);
    }

    r = EVP_EncodeBlock(sid, bytes, byteslen);
    if (r == -1) {
        handle_error(__FUNCTION__, __LINE__);
    }
    assert(r == SID_LEN);

    return std::string((char*)sid);
}

void auth::SessionManager::handle_error(const std::string &f, const int &l) {
    std::ostringstream error_msg;
    unsigned long e = ERR_get_error();
    error_msg << "SSL Error (" << ERR_error_string(e, NULL) << ") "
              << "on function " << f << ", file " __FILE__
              << ", line " << l << std::endl;

    log_error(NULL, (char*)error_msg.str().c_str());
    exit(1);
}
