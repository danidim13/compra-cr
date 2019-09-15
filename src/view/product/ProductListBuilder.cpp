//
// Created by daniel on 15/09/19.
//

#include <sstream>
#include "ProductListBuilder.h"

view::ProductListBuilder::ProductListBuilder(const std::string &title, const std::list<view::ProductCard> &cards) :
        title(title), cards(cards) {}

std::string view::ProductListBuilder::build_content() {
    std::ostringstream content;

    //
    content << R"(
            <!-- Inicio Ofertas/últimos artículos/Resultado de búsqueda -->
            <div class="row bg-white latest-items">
                <div class="col">
                    <div class="my-3"></div>
                    <div class="row">
                        <div class="col">
                            <h1 class="my-3" style="font-family: 'Alfa Slab One', cursive;">
)";
    content << title << R"(
                            </h1>
                        </div>
                    </div>)";

    int rows = cards.size() / CARDS_PER_ROW;
    auto it = cards.begin();

    for (int i = 0; i < rows; ++i) {

        //////////////
        // DECK ROW //
        content << R"(
                    <div class="row">
                        <!-- Inicio del card deck-->
                        <div class="card-deck mx-3 my-3">
)";

        for (int j = 0; j < CARDS_PER_ROW; j++) {
            if (it != cards.end()) {
                content << *it << std::endl;
                ++it;
            } else {
                content << ProductCard();
            }
        }

        content << R"(
                        </div>
                        <!-- Fin del card deck   -->
                    </div>
)";
        //          //
        //////////////

    }

    content << R"(

                    <div class="my-5"></div>
                </div>
            </div>
            <!-- Fin Ofertas/últimos artículos -->
)";
    return content.str();
}
