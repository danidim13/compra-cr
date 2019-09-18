//
// Created by daniel on 13/09/19.
//

#ifndef APP_ECOMMERCE_REQUEST_H
#define APP_ECOMMERCE_REQUEST_H


#include <string>
#include <map>

namespace http{

    class Request {
    public:
        bool valid;
        std::string m_Method;
        std::map<std::string, std::string> m_queryMap;
        size_t m_ContentLength;
        std::string m_ContentType;
        std::string m_Content;
        std::string m_RequestUri;
        std::string m_ScriptName;
        std::string m_PathInfo;
        std::string m_HttpCookie;
        std::map<std::string, std::string> m_CookieMap;

        std::string m_Action;
    };

}

#endif //APP_ECOMMERCE_REQUEST_H
