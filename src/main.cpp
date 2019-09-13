#include <stdio.h>
#include <stdlib.h>
#include "util/log.h"
#include "http/http.h"
#include "http/Router.h"
#include "view/PageBuilder.h"


int main(int argc, char* argv[]) {

    http::giArgc = argc;
    http::gsArgv = argv;

    http::Router* router = http::get_router();
    router->parse_request();

    printf("Content-type: text/html; charset=utf-8\n\n");
    // printf("<h1>Hello, World!</h1>\n");
    PageBuilder pageBuilder;
    printf("%s\n", pageBuilder.build_document().c_str());
    return 0;
}