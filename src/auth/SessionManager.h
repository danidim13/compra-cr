//
// Created by daniel on 17/10/19.
//

#ifndef APP_ECOMMERCE_SESSIONMANAGER_H
#define APP_ECOMMERCE_SESSIONMANAGER_H


#include <string>
#include <map>

namespace auth {

    const int SID_LEN = 32; // Debe ser multiplo de 4

    class SessionManager {
    public:
        SessionManager();

        void setUser(const unsigned int id);
        unsigned  int getUser() const;

        void setShoppingCart(const std::string &cart, const bool &purchased = false);
        std::string getShoppingCart() const;

        std::string getCookie() const;

    private:

        std::string genSID();
        void handle_error(const std::string &f, const int &l);

        unsigned int user_id;
        std::string shopping_cart;
        bool purchase_finalized;

    };

}

#endif //APP_ECOMMERCE_SESSIONMANAGER_H
