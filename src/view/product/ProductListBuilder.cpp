//
// Created by daniel on 15/09/19.
//

#include <sstream>
#include "ProductListBuilder.h"

view::ProductListBuilder::ProductListBuilder(const std::string &title, const std::list<view::ProductCard> &cards) :
        PageBuilder(title), cards(cards) {}

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

    int rows = (cards.size() / CARDS_PER_ROW) + (cards.size() % CARDS_PER_ROW != 0);
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

std::string view::ProductListBuilder::build_body() {
    std::ostringstream body;

    body << R"(
    <!-- Inicio contenido -->
    <main>
        <div class="container">

            <!-- Inicio banner title -->
            <div class="row" style="background-image: linear-gradient(141deg, #9fb8ad 0%, #1fc8db 51%, #2cb5e8 75%);">
                <div class="col">
                    <div class="py-5">
                        <div class="my-5">
                            <h1 class="text-white text-center" style="font-family: 'Alfa Slab One', cursive;">Compra y venta de artículos en línea</h1>
                        </div>
                    </div>
                </div>
            </div>
            <!-- Fin banner title -->

            <!-- Inicio barra de búsqueda -->
            <div class="row bg-white">
                <div class="col my-4">
                    <form action="/product/list" method="GET" role="search">
                        <div class="form-group row">
                            <div class="col-10">
                                <input type="text" name="search" class="form-control" placeholder="Búsqueda de productos">
                            </div>
                            <div class="col-2">
                                <button type="submit" class="btn btn-primary btn-block">Buscar</button>
                            </div>
                        </div>
                    </form>
                </div>
            </div>
            <!-- Fin barra de búsqueda -->
)";
    body << build_content();

    body << R"(

        </div>
    </main>
    <!-- Fin contenido -->
)";
    return body.str();
}
