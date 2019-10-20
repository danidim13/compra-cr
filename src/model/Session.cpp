//
// Created by daniel on 20/10/19.
//

#include "Session.h"
#include "../validate/StringValidator.h"
#include "ConnectionHandler.h"

model::Session::Session() {
    table = "sessions";
    m_cols = {
            {"id", EntityAttr("")},
            {"user_id", EntityAttr(0u)},
            {"data", EntityAttr("")},
            {"time_created", EntityAttr(0ll)},
            {"time_last_access", EntityAttr(0ll)},
            {"remote_ip", EntityAttr("")},
            {"user_agent", EntityAttr("")},
    };
}

std::string model::Session::id() {
    return m_cols["id"].attrStr;
}

void model::Session::id(std::string val) {
    m_cols["id"] = val;
}

unsigned int model::Session::user_id() {
    return m_cols["user_id"].attrValue.u;
}

void model::Session::user_id(unsigned int val) {
    m_cols["user_id"].attrValue.u = val;
}

std::string model::Session::data() {
    return m_cols["data"].attrStr;
}

void model::Session::data(std::string val) {
    m_cols["data"].attrStr = val;
}

time_t model::Session::time_created() {
    return m_cols["time_created"].attrValue.l;
}

void model::Session::time_created(time_t val) {
    m_cols["time_created"].attrValue.l = val;
}

time_t model::Session::time_last_access() {
    return m_cols["time_last_access"].attrValue.l;;
}

void model::Session::time_last_access(time_t val) {
    m_cols["time_last_access"].attrValue.l = val;
}

std::string model::Session::remote_ip() {
    return m_cols["remote_ip"].attrStr;
}

void model::Session::remote_ip(std::string val) {
    m_cols["remote_ip"].attrStr = val;
}

std::string model::Session::user_agent() {
    return m_cols["user_agent"].attrStr;
}

void model::Session::user_agent(std::string val) {
    m_cols["user_agent"].attrStr = val;
}

std::vector<model::Session> model::Session::find(std::string id) {
    std::ostringstream query;
    query << "SELECT * FROM sessions WHERE "
          << "id LIKE '" << id << "' LIMIT 1";
    std::vector<model::Session> sessions(model::ConnectionHandler::executeQuery<model::Session>(query.str()));
    return sessions;
}

void model::Session::clear(std::string id) {
    std::ostringstream query;
    query << "DELETE FROM sessions WHERE id LIKE '" << id << "'";
    model::ConnectionHandler::execute(query.str());
}

void model::Session::update(const std::vector<std::string> &select) {
    std::ostringstream query, values;


    size_t len = select.size();
    for (auto col: select) {
        --len;
        switch (m_cols[col].attrType) {
            case model::INT: {
                values << col << "=" << m_cols[col].attrValue.i;
                break;
            }
            case model::UINT: {
                values << col << "=" << m_cols[col].attrValue.u;
                break;
            }
            case model::LONG: {
                values << col << "=" << m_cols[col].attrValue.l;
                break;
            }
            case model::STRING: {
                values << col << "='" << m_cols[col].attrStr << "'";
                break;
            }
            default:
                break;
        }
        if (len > 0) {
            values << ", ";
        }
    }

    query << "UPDATE " << table << " SET " << values.str() << " WHERE id LIKE '" << m_cols["id"].attrStr << "'";
    model::ConnectionHandler::execute(query.str());

}

void model::Session::logout(const std::string &id) {
    std::ostringstream query;
    query << "UPDATE sessions SET user_id=NULL WHERE id LIKE '" << id << "'";
    model::ConnectionHandler::execute(query.str());
}
