//
// Created by daniel on 14/09/19.
//

#include "User.h"

model::User::User()
{

    m_cols = {
        {"id", EntityAttr(0u)},
        {"username", EntityAttr(std::string(""))},
        {"surname", EntityAttr(std::string(""))},
        {"last_name", EntityAttr(std::string(""))},
        {"email", EntityAttr(std::string(""))},
        {"phone", EntityAttr(std::string(""))}
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

