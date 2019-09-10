//
// Created by daniel on 03/09/19.
//

#ifndef APP_ECOMMERCE_HTTP_H
#define APP_ECOMMERCE_HTTP_H

#include "Router.h"

namespace http {
    extern char *gpMethod;
    extern char *gpQueryString;
    extern char *gpPathInfo;
    extern int   giArgc;
    extern char **gsArgv;

    extern Router *gRouter;

    Router* get_router();
}

#endif //APP_ECOMMERCE_HTTP_H
