//
// Created by daniel on 10/09/19.
//

#ifndef APP_ECOMMERCE_PAGEBUILDER_H
#define APP_ECOMMERCE_PAGEBUILDER_H

#include <string>

// #define TEMPLATE_PATH "webroot/html/index.html"

namespace view {

class PageBuilder {
public:
    PageBuilder();
    PageBuilder(const std::string &title);

    virtual std::string build_document();
    virtual std::string build_head();
    virtual std::string build_header();
    virtual std::string build_body();
    virtual std::string build_content();
    virtual std::string build_footer();

    void setUserInfo(const std::string &name, const int &cartItems);

protected:
    std::string title;
    std::string username;
    bool logged;
};

}

#endif //APP_ECOMMERCE_PAGEBUILDER_H
