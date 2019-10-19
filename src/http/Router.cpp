//
// Created by daniel on 03/09/19.
//

#include "Router.h"
#include "../util/log.h"
#include "../util/decode.h"
#include "http.h"
#include "Request.h"
#include "../util/cookie.h"
#include <string>
#include <vector>
#include <cstring>
#include <iostream>


http::Router::Router() {
//    log_info(NULL, "Created Router");
    http::gpMethod = getenv("REQUEST_METHOD"); // GET o POST
    http::gpQueryString = getenv("QUERY_STRING"); // ?param1=val&param2=val+compuesto
    http::gpContentLength = getenv("CONTENT_LENGTH"); // para POST, lo que hay que leer de stdin
    http::gpContentType = getenv("CONTENT_TYPE"); // para POSt, tipo de lo que sea lea
    http::gpRequestUri = getenv("REQUEST_URI"); // dirección completa p.ej. /ProyectoSeguridad/article/view?id=1
    http::gpScriptName = getenv("SCRIPT_NAME"); // dirección desde htdocs del ejecutable (se puede usar para construir links)
    http::gpPathInfo = getenv("PATH_INFO"); // dirección relativa (desde script)
    http::gpHttpCookie = getenv("HTTP_COOKIE"); // dirección relativa (desde script)

    http::gpRemoteAddr = getenv("REMOTE_ADDR"); // dirección relativa (desde script)
    http::gpRemoteHost = getenv("REMOTE_HOST"); // dirección relativa (desde script)
    http::gpUserAgent = getenv("HTTP_USER_AGENT"); // dirección relativa (desde script)

    char sbuf[1024];
    sprintf(sbuf, "Request received\nRequest method: %s\nQuery string: %s\nContent length: %s\n"
                  "Content type: %s\nRequest URI: %s\nScript name: %s\nPath info: %s\nCookie: %s\n"
                  "Remote addr: %s\nRemote host: %s\nUser-agent: %s\nargc: %d\n",
                  gpMethod, gpQueryString, gpContentLength,
                  gpContentType, gpRequestUri, gpScriptName, gpPathInfo, gpHttpCookie,
                  gpRemoteAddr, gpRemoteHost, gpUserAgent,
                  giArgc);
    log_debug(NULL, sbuf);
}

void http::Router::parse_request() {

    m_request.valid = false;

    if (strcmp(gpMethod, "GET") == 0) {

        m_request.m_Method = std::string(gpMethod);
        log_debug(NULL, (char*)"GET request");

    } else if (strcmp(gpMethod, "POST") == 0) {

        m_request.m_Method = std::string(gpMethod);
        log_debug(NULL, (char*)"POST request");

        if (gpContentLength) {
            if (atoll(gpContentLength) > 0) {
                log_debug(NULL, "Leyendo datos del post");

                sscanf(gpContentLength, "%zu", &m_request.m_ContentLength);
                char buffer[m_request.m_ContentLength + 1];
                std::cin.read(buffer, m_request.m_ContentLength);
                buffer[m_request.m_ContentLength] = '\0';
                m_request.m_Content = std::string(buffer);

                // Hacer algo con CONTENT_TYPE?
                m_request.m_ContentType = std::string(gpContentType);

                log_debug(NULL, (char*)std::string("Content-type: ").append(m_request.m_ContentType).c_str());
                log_debug(NULL, (char*)std::string("Content: ").append(m_request.m_Content).c_str());

            } else if (atoll(gpContentLength) == 0) {
                m_request.m_ContentLength = 0;
                m_request.m_Content = "";
            } else {
                log_error(NULL, (char *) "Unexpected CONTENT_LENGTH value");
                exit(1);
            }
        } else {
            log_error(NULL, (char *) "No CONTENT_LENGTH value");
            exit(1);
        }
    } else {
        std::string error_msg("Unrecognized m_request method: ");
        error_msg.append(gpMethod);
        log_error(NULL, (char*)error_msg.c_str());
        exit(1);
    }

//    this->parse_query_string();
    m_request.m_queryMap = split_query(http::gpQueryString);

    m_request.m_RequestUri = std::string(gpRequestUri);
    m_request.m_ScriptName = std::string(gpScriptName);

    if (gpHttpCookie) {
        m_request.m_HttpCookie = std::string(gpHttpCookie);
        m_request.m_CookieMap = split_cookie(gpHttpCookie);
    } else {
        m_request.m_HttpCookie = "";
    }

    m_request.m_PathInfo = std::string(gpPathInfo);
    auto alias_it = m_aliases.find(m_request.m_PathInfo);
    if (alias_it != m_aliases.end()) {
        m_request.m_Action = alias_it->second;
    } else {
        m_request.m_Action = m_request.m_PathInfo;
    }
    m_request.valid = true;
    return;
}

unsigned long http::Router::parse_query_string() {

    char sbuf[1024];

    std::string query(http::gpQueryString);

    if (query.length() > 0){
        sprintf(sbuf, "Query string len: %lu, contents: %s", query.length(), query.c_str());
        log_debug(NULL, sbuf);

        std::vector<std::string> tokens;

        char *query_cpy = strdup(query.c_str());
        char *token;

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
            std::string key(token);

            token = strtok(NULL, "=");
            if (token) {
                std::string value(token);
                m_request.m_queryMap[key] = value;
            } else {
                sprintf(sbuf, "Invalid format on query token \"%s\"", it->c_str());
                log_error(NULL, sbuf);
            }

            free(token_cpy);
        }

    }
    return m_request.m_queryMap.size();
}

http::Request * http::Router::get_request() {
    return &m_request;
}

http::Response * http::Router::get_response() {
    return &m_response;
}
