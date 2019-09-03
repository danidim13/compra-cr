#include <cstdio.h>
#include "util/log.h"

int main() {
    log_warning(NULL, "Peligro inminente");

    printf("Content-type: text/html; charset=utf-8\n\n");
    printf("<h1>Hello, World!</h1>\n");
    return 0;
}