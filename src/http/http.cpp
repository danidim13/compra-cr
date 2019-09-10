//
// Created by daniel on 03/09/19.
//

#include "http.h"

http::Router *http::gRouter = nullptr;
char *http::gpMethod = nullptr;
char *http::gpPathInfo = nullptr;
char *http::gpQueryString = nullptr;
int http::giArgc;
char **http::gsArgv;

http::Router *http::get_router() {
    if (http::gRouter == nullptr) {
        http::gRouter = new Router();
    }
    return http::gRouter;
}
