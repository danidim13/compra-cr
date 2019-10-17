//
// Created by daniel on 14/09/19.
//

#ifndef APP_ECOMMERCE_CONTROLLER_H
#define APP_ECOMMERCE_CONTROLLER_H


#include <string>
#include "Router.h"

#define LATEST 10

namespace http {

class Controller {
public:
    Controller();

    void processAction();

protected:
    Router *router;
private:
    void processGetReq(const Request &request);
    void processPostReq(const Request &request);
};

}

#endif //APP_ECOMMERCE_CONTROLLER_H
