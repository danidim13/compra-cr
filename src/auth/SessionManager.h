//
// Created by daniel on 17/10/19.
//

#ifndef APP_ECOMMERCE_SESSIONMANAGER_H
#define APP_ECOMMERCE_SESSIONMANAGER_H


#include <string>
#include <map>
#include "../nlohmann/json_fwd.hpp"
#include "../model/Session.h"
#include "../nlohmann/json.hpp"

#define SESSION_KEEP_ALIVE_MIN 40

namespace auth {

    const int SID_LEN = 16; // Debe ser multiplo de 4

    class SessionManager {
    public:
        SessionManager();

        void initFromCookie(const std::map<std::string, std::string> &cookie);

        void setUser(const unsigned int id);
        unsigned  int getUser() const;

        void setShoppingCart(const std::vector<unsigned int> &cart, const bool &purchased = false);
        std::vector<unsigned int> getShoppingCart() const;

        std::string getCookie() const;
        void pushSessionData();

        nlohmann::json session_data;

    private:

        std::string genSID();
        void newSession();
        void handle_error(const std::string &f, const int &l);

        void fetchSessionData(std::string id);
        void setFromEntity(model::Session session);

        std::string sessionId;
        unsigned int user_id;
        time_t created;
        time_t last_access;
        std::string remote_ip;
        std::string user_agent;

//        std::vector<int> shopping_cart;

        bool purchase_finalized;
        bool new_session;

    };

}

#endif //APP_ECOMMERCE_SESSIONMANAGER_H
