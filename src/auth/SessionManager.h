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

    private:

        std::string genSID();
        void handle_error(const std::string &f, const int &l);

    };

}

#endif //APP_ECOMMERCE_SESSIONMANAGER_H
