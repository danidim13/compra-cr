//
// Created by daniel on 16/09/19.
//

#include "decode.h"
#include <cstring>
#include <cstdlib>
#include <vector>
#include "log.h"

std::string url_decode(char *src) {
    size_t src_len = strlen(src);
    char dest[src_len+1];

    url_decode(src, dest);
    return std::string(dest);
}

size_t url_decode(char *src, char *dest) {

    char hex_buf[3] = "00";
    char *pDest = dest;
    size_t i;

    for (i = 0; i < strlen(src); i++) {
        if (src[i] == '+') {
            *pDest = ' ';
        } else if (src[i] == '%') {
            memcpy(hex_buf, src+i+1, 2);
            *pDest = (char)strtol(hex_buf, NULL, 16);
            i += 2;
        } else {
            *pDest = src[i];
        }
        ++pDest;
    }
    *pDest = '\0';
    return i;
}

std::map<std::string, std::string> split_query(char *src) {
    std::vector<std::string> tokens;
    std::map<std::string, std::string> queryMap;

    char *query_cpy = strdup(src);
    char *token;

    // Partir string en tokens por '&'
    token = strtok(query_cpy, "&");
    while (token != NULL) {
        tokens.push_back(std::string(token));
        token = strtok(NULL, "&");
    }
    free(query_cpy);

    log_debug(NULL, (char*)"Tokens:");

    for (auto it = tokens.begin(); it != tokens.end(); ++it) {

        char *token_cpy = strdup(it->c_str());

        log_debug(NULL, token_cpy);

        token = strtok(token_cpy, "=");
        url_decode(token, token);
        std::string key(token);

        token = strtok(NULL, "=");
        if (token) {
            url_decode(token, token);
            std::string value(token);
            queryMap[key] = value;
        } else {
            char sbuf[1024];
            sprintf(sbuf, "Empty or invalid format on query token \"%s\"", it->c_str());
            log_warning(NULL, sbuf);
            queryMap[key] = "";
        }

        free(token_cpy);
    }
    return queryMap;
}
