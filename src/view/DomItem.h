//
// Created by daniel on 10/09/19.
//

#ifndef APP_ECOMMERCE_DOMITEM_H
#define APP_ECOMMERCE_DOMITEM_H


#include <unordered_map>
#include <list>
#include <string>

class DomItem {

public:

    enum DomContentType {
        NONE,
        SIMPLE,
        COMPLEX,
    };

    DomItem(const std::string &mTag, const std::unordered_map<std::string, std::string> &mAttributes,
            const std::list<DomItem> &mChildren, DomItem::DomContentType mContentType, DomItem *mParent);

    std::string m_tag;
    std::unordered_map<std::string, std::string> m_attributes;
    DomContentType m_contentType;

    std::string m_content;

    std::list<DomItem> m_children;

    DomItem* m_parent;

    std::string to_string();
};


#endif //APP_ECOMMERCE_DOMITEM_H
