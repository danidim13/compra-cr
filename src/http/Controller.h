//
// Created by daniel on 14/09/19.
//

#ifndef APP_ECOMMERCE_CONTROLLER_H
#define APP_ECOMMERCE_CONTROLLER_H


#include <string>
#include <memory>
#include "Router.h"
#include "../auth/SessionManager.h"
#include "../view/PageBuilder.h"

#define LATEST 10

namespace http {

class Controller {
public:
    Controller();

    void processAction();

protected:
    Router *router;
    auth::SessionManager sessionManager;
    std::unique_ptr<view::PageBuilder> pageView;

    void Found(std::string location);
    void SeeOther(std::string location);
    void BadRequest(std::string location);
    void NotFound();
    void BadRequest();
    void MethodNotAllowed();

private:
    bool validateReq();
    void setSession();
    void processReq(const Request &request);
    void processGetReq(const Request &request);
    void processPostReq(const Request &request);
    void refreshSession();
    void makeResponse();

};

}

#endif //APP_ECOMMERCE_CONTROLLER_H
