//
// Created by daniel on 03/09/19.
//

#ifndef APP_ECOMMERCE_HTTP_H
#define APP_ECOMMERCE_HTTP_H

namespace http {
    class Router;

    extern char *gpMethod;
    extern char *gpQueryString;
    extern char *gpContentLength;
    extern char *gpContentType;
    extern char *gpRequestUri;
    extern char *gpScriptName;
    extern char *gpPathInfo;
    extern char *gpHttpCookie;
    extern int   giArgc;
    extern char **gsArgv;

    extern Router *gRouter;

    Router* get_router();
}

#endif //APP_ECOMMERCE_HTTP_H
