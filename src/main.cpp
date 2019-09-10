#include <stdio.h>
#include <stdlib.h>
#include "util/log.h"
#include "http/http.h"

int main(int argc, char* argv[]) {

    http::giArgc = argc;
    http::gsArgv = argv;

    http::Router* router = http::get_router();
    router->parse_request();

    printf("Content-type: text/html; charset=utf-8\n\n");
    printf("<h1>Hello, World!</h1>\n");
    return 0;
}