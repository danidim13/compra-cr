//
// Created by daniel on 10/09/19.
//

#ifndef APP_ECOMMERCE_DOMITEM_H
#define APP_ECOMMERCE_DOMITEM_H


#include <unordered_map>
#include <list>
#include <string>
#include <ostream>

class DomItem {
public:

    enum DomContentType {
        NONE,       /** <tag/> */
        OPEN,       /** <tag>  */
        SIMPLE,     /** <tag>content</tag> */
        COMPLEX,    /** <tag>{foreach child}</tag> */
    };

    /**
     * Contructor sin contenido
     * @param mTag          Tag html, p. ej. "a", "button", etc...
     * @param mAttributes   Atributos, p. ej. class y style
     * @param mContentType  Tipo de contenido, ya sea <tag></tag>, <tag/> o <tag><c1></c1><c2></c2>...</tag>
     * @param mParent       Objeto padre, de ser necesario
     */
    DomItem(const std::string &mTag, const std::unordered_map<std::string, std::string> &mAttributes,
            DomItem::DomContentType mContentType, DomItem *mParent);

    typedef std::unordered_map<std::string, std::string> m_attributes_type;
    typedef std::list<DomItem> m_children_type;

    std::string m_tag;
    m_attributes_type m_attributes;
    DomContentType m_contentType;
    std::string m_content;
    m_children_type m_children;
    DomItem* m_parent;

    std::string to_string() const;

    friend std::ostream &operator<<(std::ostream &os, const DomItem &item);
};


#endif //APP_ECOMMERCE_DOMITEM_H
