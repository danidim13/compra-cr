//
// Created by daniel on 10/09/19.
//

#ifndef APP_ECOMMERCE_PAGEBUILDER_H
#define APP_ECOMMERCE_PAGEBUILDER_H

#include <string>

// #define TEMPLATE_PATH "webroot/html/index.html"

class PageBuilder {
public:
    std::string build_document();
    std::string build_head();
    std::string build_header();
    std::string build_body();
    std::string build_footer();
    std::string build_product_cards();
};



#endif //APP_ECOMMERCE_PAGEBUILDER_H
