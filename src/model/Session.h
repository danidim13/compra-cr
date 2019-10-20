//
// Created by daniel on 20/10/19.
//

#ifndef APP_ECOMMERCE_SESSION_H
#define APP_ECOMMERCE_SESSION_H


#include "model.h"
#include <ctime>

namespace model {

class Session : public Entity {
public:
    Session();

    std::string id();
    void id(std::string val);

    unsigned int user_id();
    void user_id(unsigned int val);

    std::string data();
    void data(std::string val);

    time_t time_created();
    void time_created(time_t val);

    time_t time_last_access();
    void time_last_access(time_t val);

    std::string remote_ip();
    void remote_ip(std::string val);

    std::string user_agent();
    void user_agent(std::string );

    void update(const std::vector<std::string> &select);

    static void logout(const std::string &id);
    static std::vector<Session> find(std::string id);
    static void clear(std::string id);


};

}

#endif //APP_ECOMMERCE_SESSION_H
