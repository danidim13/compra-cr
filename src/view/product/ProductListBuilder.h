//
// Created by daniel on 15/09/19.
//

#ifndef APP_ECOMMERCE_PRODUCTLISTBUILDER_H
#define APP_ECOMMERCE_PRODUCTLISTBUILDER_H


#include "../PageBuilder.h"
#include "../ProductCard.h"

#define CARDS_PER_ROW 3

namespace view {

class ProductListBuilder: public PageBuilder {
public:
    ProductListBuilder(const std::string &title, const std::list<ProductCard> &cards);

    std::string build_body() override;
    std::string build_content() override;

public:

    std::list<view::ProductCard> cards;
};

}

#endif //APP_ECOMMERCE_PRODUCTLISTBUILDER_H
