//
// Created by daniel on 13/09/19.
//

#ifndef APP_ECOMMERCE_REQUEST_H
#define APP_ECOMMERCE_REQUEST_H


#include <string>

namespace http{

    class Request {
        std::string m_Method;
        std::string m_QueryString;
        std::string m_ContentLength;
        std::string m_ContentType;
        std::string m_Content;
        std::string m_RequestUri;
        std::string m_ScriptName;
        std::string m_PathInfo;
    };

}

#endif //APP_ECOMMERCE_REQUEST_H
