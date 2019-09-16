#include <stdio.h>
#include <stdlib.h>
#include "util/log.h"
#include "http/http.h"
#include "http/Controller.h"
//#include "http/Router.h"
//#include "view/PageBuilder.h"


int main(int argc, char* argv[]) {

    http::giArgc = argc;
    http::gsArgv = argv;

//    printf("Content-type: text/html; charset=utf-8\n\n");

//    http::Router* router = http::get_router();
//    router->parse_request();


    http::Controller controller;
    controller.processAction();

    // printf("<h1>Hello, World!</h1>\n");
    return 0;
}