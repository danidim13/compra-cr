//
// Created by daniel on 14/09/19.
//

#include <sstream>
#include "User.h"
#include "ConnectionHandler.h"
#include "../validate/StringValidator.h"
#include "../auth/PasswordHasher.h"

model::User::User()
{

    table = "users";
    m_cols = {
        {"id", EntityAttr(0u)},
        {"username", EntityAttr(std::string(""))},
        {"surname", EntityAttr(std::string(""))},
        {"last_name", EntityAttr(std::string(""))},
        {"email", EntityAttr(std::string(""))},
        {"phone", EntityAttr(std::string(""))},
        {"password", EntityAttr(std::string(""))}
    };

    default_validator.fields = {
        {"username", std::make_shared<validate::StringValidator>(
                validate::REGEX_USERNAME, 1, 50 )},
        {"password", std::make_shared<validate::StringValidator>(
                validate::REGEX_PASSWORD, 8, 50 )},
        {"surname", std::make_shared<validate::StringValidator>(
                validate::REGEX_SPANISH_NAMES, 1, 50 )},
        {"last_name", std::make_shared<validate::StringValidator>(
                validate::REGEX_SPANISH_NAMES, 1, 50 )},
        {"email", std::make_shared<validate::StringValidator>(
                validate::REGEX_EMAIL, 1, 50 )},
        {"phone", std::make_shared<validate::StringValidator>(
                validate::REGEX_PHONE, 1, 50 )},
    };
}

unsigned int model::User::id() {
    return m_cols["id"].attrValue.u;
}

std::string model::User::username() {
    return m_cols["username"].attrStr;
}

std::string model::User::surname() {
    return m_cols["surname"].attrStr;
}

std::string model::User::last_name() {
    return m_cols["last_name"].attrStr;
}

std::string model::User::email() {
    return m_cols["email"].attrStr;
}

std::string model::User::phone() {
    return m_cols["phone"].attrStr;
}

std::string model::User::password() {
    return m_cols["password"].attrStr;
}

std::pair<bool, std::string> model::User::login(const std::string &username, const std::string &password) {
    User user;
    std::pair<bool, std::string> res;
    std::ostringstream query, msg;
    query << "SELECT id, username, password FROM " << user.table << " WHERE username LIKE '" << username << "' LIMIT 1";

    std::vector<User> auth_user(ConnectionHandler::executeQuery<User>(query.str(), {"id", "username", "password"}));
    if (auth_user.size() == 1) {

        auth::PasswordHasher hasher;
        if (hasher.passwordVerify(password, auth_user[0].password())) {
            msg << "Login succes for " << username << " with id " << auth_user[0].id();
            res.first = true;
            res.second = std::to_string(auth_user[0].id());
            log_info(NULL, (char*) msg.str().c_str());

        } else {
            res.first = false;
            msg << "Unsuccessful login attempt for " << username;
            log_warning(NULL, (char*) msg.str().c_str());
            // FIXME: no dar esta información?
            res.second = std::string("Usuario+o+contraseña+incorretos");
        }

    } else {
        res.first = false;
        msg << "Unsuccessful login attempt for " << username;
        log_warning(NULL, (char*) msg.str().c_str());
        // FIXME: no dar esta información?
        res.second = std::string("Usuario+inexistente");
    }

    return res;
}

validate::MapValidator model::User::LoginValidator() {
    return validate::MapValidator({
        {"username", std::make_shared<validate::StringValidator>(
              validate::REGEX_USERNAME, 1, 50 )},
        {"password", std::make_shared<validate::StringValidator>(
              validate::REGEX_PASSWORD, 8, 50 )},
    });
}

