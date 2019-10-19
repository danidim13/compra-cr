//
// Created by daniel on 19/10/19.
//

#ifndef APP_ECOMMERCE_NOTFOUNDPAGEBUILDER_H
#define APP_ECOMMERCE_NOTFOUNDPAGEBUILDER_H


#include "../PageBuilder.h"

namespace view {

class NotFoundPageBuilder : public PageBuilder {
public:
    std::string build_document() override;

};

}

#endif //APP_ECOMMERCE_NOTFOUNDPAGEBUILDER_H
