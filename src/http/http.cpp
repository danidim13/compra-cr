//
// Created by daniel on 03/09/19.
//

#include "http.h"
#include "Router.h"

http::Router *http::gRouter = nullptr;
char *http::gpMethod = nullptr;
char *http::gpQueryString = nullptr;
char *http::gpContentLength = nullptr;
char *http::gpContentType = nullptr;
char *http::gpRequestUri = nullptr;
char *http::gpScriptName = nullptr;
char *http::gpPathInfo = nullptr;
char *http::gpHttpCookie = nullptr;
int http::giArgc;
char **http::gsArgv;

http::Router *http::get_router() {
    if (http::gRouter == nullptr) {
        http::gRouter = new Router();
    }
    return http::gRouter;
}
