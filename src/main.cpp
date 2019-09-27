#include <stdio.h>
#include <stdlib.h>
#include "util/log.h"
#include "http/http.h"
#include "http/Controller.h"
//#include "http/Router.h"
//#include "view/PageBuilder.h"


int main(int argc, char* argv[]) {
//    freopen("checkout.txt","r",stdin);

    http::giArgc = argc;
    http::gsArgv = argv;

    http::Controller controller;
    controller.processAction();

    return 0;
}