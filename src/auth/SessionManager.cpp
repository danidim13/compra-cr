//
// Created by daniel on 17/10/19.
//

#include "SessionManager.h"
#include "../util/log.h"
#include "../util/cookie.h"
#include "../validate/StringValidator.h"
#include "../model/Session.h"
#include "../http/http.h"
#include <openssl/conf.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <cassert>
#include <sstream>

auth::SessionManager::SessionManager() : user_id(0), shopping_cart("") {}

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

void auth::SessionManager::setUser(const unsigned int id) {
    user_id = id;
    if (user_id == 0) {
        model::Session::logout(sessionId);
    }
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

    /*
    if (user_id > 0) {
        cookie << "Set-Cookie: " << "user_id=" << user_id << "; Path=/; Expires=" << renewed_time() << std::endl;
    } else if (user_id == 0) {
        cookie << "Set-Cookie: " << "user_id=none" << "; Path=/; Expires=" << expired_time() << std::endl;
    }
     */

    if (purchase_finalized) {
        cookie << "Set-Cookie: " << "shopping_cart=" << shopping_cart << "; Path=/; Expires=" << renewed_time(0, 1) << std::endl;
    } else {
        cookie << "Set-Cookie: " << "shopping_cart=" << shopping_cart << "; Path=/; Expires=" << renewed_time() << std::endl;
    }

    cookie << "Set-Cookie: " <<  "sid=" << sessionId << "; Path=/; Expires=" << renewed_time() << std::endl;
    log_debug(NULL, (char*)"Cookie string");
    log_debug(NULL, (char*)cookie.str().c_str());
    return cookie.str();
}

std::string auth::SessionManager::getShoppingCart() const {
    return shopping_cart;
}


void auth::SessionManager::initFromCookie(const std::map<std::string, std::string> &cookie) {

    validate::StringValidator sessionIdValidator(validate::REGEX_B64, SID_LEN, SID_LEN);
    if (cookie.find("sid") != cookie.end()) {
        sessionId = cookie.find("sid")->second;

        if (sessionIdValidator.validate(sessionId).first) {
            // Sesión posiblemente establecida

            fetchSessionData(sessionId);

        } else {
            // Sesión nueva con posibildiad de tampering
            std::ostringstream msg;
            msg << "Error formato de SID: " << sessionId << " (" << sessionIdValidator.validate(sessionId).second << ")";
            log_error(NULL, (char*)msg.str().c_str());
            sessionId = genSID();
            new_session = true;
        }
    } else {
        // Sesión nueva
        log_info(NULL, "New session started");
        sessionId = genSID();
        new_session = true;
        log_info(NULL, (char*)sessionId.c_str());
    }

    /*
    // Validación/sanitización implícita de user_id en la conversión
    if (cookie.find("user_id") != cookie.end()) {
        setUser(strtoul(cookie.find("user_id")->second.c_str(), NULL, 10));
    } else {
        setUser(0);
    }

    if (cookie.find("shopping_cart") != cookie.end()) {
        std::string items(cookie.find("shopping_cart")->second);
        validate::StringValidator itemsValidator(validate::REGEX_CART_ITEMS, 1, 100);
        if (itemsValidator.validate(items).first) {
            setShoppingCart(items);
        }
    }
    */

}

void auth::SessionManager::fetchSessionData(std::string id) {

    log_debug(NULL, (char*)"Fetching session from database");
    std::vector<model::Session> sessions(model::Session::find(id));

    if (sessions.size() == 1) {
        // Todo bien
        time_t now = time(NULL);
        last_access = sessions[0].time_last_access();
        if (difftime(now, last_access) > 60 * SESSION_KEEP_ALIVE_MIN) {
            log_debug(NULL, (char*)"Session expired");
            sessionId = genSID();
            new_session = true;
            model::Session::clear(id);
        } else {
            setFromEntity(sessions[0]);
        }
    } else {
        // Posibilidad de tampering o sesión expirada automáticamente
        log_debug(NULL, (char*)"No session found, creating new");
        sessionId = genSID();
        new_session = true;
    }
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

void auth::SessionManager::setFromEntity(model::Session session) {

    user_id = session.user_id();
    created = session.time_created();

    remote_ip = session.remote_ip();
    if (remote_ip.compare(http::gpRemoteAddr) != 0) {
        // Hacer algo?
        std::ostringstream msg;
        msg << "Cambio inesperado de IP en sesión " << sessionId << " para usuario " << user_id
            << ": " << remote_ip << " -> " << std::string(http::gpRemoteAddr);
        remote_ip = http::gpRemoteAddr;
    }

    user_agent = session.user_agent();
    if (user_agent.compare(http::gpUserAgent) != 0) {
        // Hacer algo?
        std::ostringstream msg;
        msg << "Cambio inesperado de agente en sesión " << sessionId << " para usuario " << user_id
            << ": " << user_agent << " -> " << std::string(http::gpUserAgent);
        user_agent = http::gpUserAgent;
    }

    session_data = nlohmann::json::parse(session.data());
    new_session = false;
}

void auth::SessionManager::pushSessionData() {

    std::vector<std::string> select = {"time_last_access", "user_agent", "remote_ip", "data"};

    model::Session entity;
    entity.id(sessionId);

    if (user_id > 0) {
        entity.user_id(user_id);
        select.push_back("user_id");
    }

    entity.time_last_access(time(NULL));
    entity.user_agent(user_agent);
    entity.remote_ip(remote_ip);
    entity.data(session_data.dump());

    if (new_session) {
        entity.time_created(time(NULL));
        select.push_back("time_created");
        select.push_back("id");

        // Insert
        entity.insert(select);
    } else {
        // Updated
        entity.update(select);
    }


}

