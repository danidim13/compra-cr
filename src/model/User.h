//
// Created by daniel on 14/09/19.
//

#ifndef APP_ECOMMERCE_USER_H
#define APP_ECOMMERCE_USER_H

#include "model.h"
#include <string>

namespace model {

class User: public Entity {
public:
    User();

    unsigned int id();  // int unsigned not null auto_increment primary key;
    std::string username(); // varchar(50) not null unique;
    std::string surname(); // varchar(50) not null;
    std::string last_name(); // varchar(50) null;
    std::string email(); // varchar(256) character set ascii not null unique;
    std::string phone(); // varchar(50) null
};
}


#endif //APP_ECOMMERCE_USER_H
