//
// Created by daniel on 10/09/19.
//

#include <sstream>
#include "DomItem.h"

DomItem::DomItem(const std::string &mTag, const std::unordered_map<std::string, std::string> &mAttributes,
                 const std::list<DomItem> &mChildren, DomItem::DomContentType mContentType,
                 DomItem *mParent = nullptr) :
        m_tag(mTag),
        m_attributes(mAttributes),
        m_children(mChildren),
        m_parent(mParent), m_contentType(mContentType) {

}

// ContentType = NONE <{tag} {attr1}="{value1}" {attr2}="{value2}"/>
// ContentType = SIMPLE <{tag} {attr1}="{value1}" {attr2}="{value2}">{std::string content}</tag>
// ContentType = COMPLEX <{tag} {attr1}="{value1}" {attr2}="{value2}"/>{std::list children}</tag>
std::string DomItem::to_string() {

    std::ostringstream builder;
    builder << "<" << m_tag;
    for (auto it = m_attributes.begin(); it != m_attributes.end(); ++it) {
        builder << " " << it->first << "=\"" << it->second << "\"";
    }
    switch (m_contentType) {
        case NONE: {
            builder << "/>";
            break;
        }
        case SIMPLE: {
            builder << ">" << m_content << "</" << m_tag << ">";
            break;
        }
        case COMPLEX: {
            // TODO
            builder << ">NOT IMPLEMENTED" << "</" << m_tag << ">";
        }
    }
    return builder.str();
}

