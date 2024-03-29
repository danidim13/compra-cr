//
// Created by daniel on 10/09/19.
//

#ifndef APP_ECOMMERCE_NAVITEM_H
#define APP_ECOMMERCE_NAVITEM_H


#include "DomItem.h"

namespace view {

class NavItem: public DomItem {
public:
    NavItem(const std::string &href, const std::string &content,
            const std::unordered_map<std::string, std::string> &mAttributes);

    NavItem(const std::string &href, const std::string &content);

public:

};

}

#endif //APP_ECOMMERCE_NAVITEM_H
