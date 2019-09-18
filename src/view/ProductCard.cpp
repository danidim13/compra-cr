//
// Created by daniel on 15/09/19.
//

#include "PageBuilder.h"
#include <sstream>
#include "ProductCard.h"

view::ProductCard::ProductCard(const unsigned int &id, const std::string &title, const std::string &detail,
                               const std::string &price) : id(id), title(
        title), detail(detail), price(price) {}

view::ProductCard::ProductCard(): id(0), title(""), detail(""), price("") {}

std::string view::ProductCard::to_string() const {
    std::ostringstream card;
    card << R"(
        <div class="card">
            <img src="https://via.placeholder.com/350x250" class="card-img-top" alt="placeholder-image">
            <div class="card-body">
                <h4 class="card-title"> )";
    card << title << std::endl;
    card << R"(
                </h4>
                <p class="card-text"> )";
    card << detail << std::endl;
    card << R"(
                </p>
            </div>
            <div class="card-body">
                <p class="card-text"> )";
    card << "₡ " << price << std::endl;
    card << R"(
                </p>
            </div>
            <div class="card-footer text-center">
                <a href="/cart/add?id=)" << id << R"(" class="btn btn-primary">
                    Agregar al carrito
                </a>
            </div>
        </div>
    )";

    return card.str();
}

std::ostream &view::operator<<(std::ostream &os, const view::ProductCard &card) {
    os << card.to_string();
    return os;
}
