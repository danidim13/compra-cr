//
// Created by daniel on 10/09/19.
//

#include "NavItem.h"

NavItem::NavItem(const std::string &href, const std::string &content,
                 const std::unordered_map<std::string, std::string> &mAttributes)
        : DomItem("a", mAttributes, DomItem::SIMPLE, nullptr) {
    this->m_content = content;
    this->m_attributes["href"] = href;
}
