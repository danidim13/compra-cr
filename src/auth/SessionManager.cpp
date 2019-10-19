//
// Created by daniel on 17/10/19.
//

#include "SessionManager.h"
#include "../util/log.h"
#include "../util/cookie.h"
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

void auth::SessionManager::setUser(const unsigned int id) {
    user_id = id;

}

unsigned int auth::SessionManager::getUser() const {
    return user_id;
}

void auth::SessionManager::setShoppingCart(const std::string &cart, const bool &purchased) {
    shopping_cart = cart;
    purchase_finalized = purchased;
}

std::string auth::SessionManager::getCookie() const {
    std::ostringstream cookie;

    if (user_id > 0) {
        cookie << "Set-Cookie:" << "user_id=" << user_id << "; Path=/;" << "Expires=" << renewed_time() << std::endl;
    } else if (user_id == 0) {
        cookie << "Set-Cookie:" << "user_id=none" << "; Path=/;" << "Expires=" << expired_time() << std::endl;
    }

    if (purchase_finalized) {
        cookie << "Set-Cookie:" << "shopping_cart=" << shopping_cart << "; Path=/;" << "Expires=" << renewed_time(0, 1);
    } else {
        cookie << "Set-Cookie:" << "shopping_cart=" << shopping_cart << "; Path=/;" << "Expires=" << renewed_time() << std::endl;
    }
    return cookie.str();
}

std::string auth::SessionManager::getShoppingCart() const {
    return shopping_cart;
}

auth::SessionManager::SessionManager() : user_id(0), shopping_cart("") {}


