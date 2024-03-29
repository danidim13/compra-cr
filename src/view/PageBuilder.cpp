//
// Created by daniel on 10/09/19.
//

#include <sstream>
#include "PageBuilder.h"
#include "ProductCard.h"
#include "NavItem.h"

std::string view::PageBuilder::build_document() {
    std::ostringstream document_builder;
    document_builder << R"(
<!DOCTYPE html>
<html lang=\"en\">
)";
    document_builder << build_head();
    document_builder << R"(
<body class="bg-light">
)";
    document_builder << build_header();
    document_builder << build_body();
    document_builder << build_footer();

    document_builder << R"(
</body>
</html>
)";


    return document_builder.str();
}

std::string view::PageBuilder::build_head() {
    return R"(
<head>
    <meta charset="UTF-8">
    <link rel="stylesheet" type="text/css" href="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css"/>
    <link href="https://fonts.googleapis.com/css?family=Alfa+Slab+One|Righteous&display=swap" rel="stylesheet">
    <title>CompraCR</title>
</head>
)";
}

std::string view::PageBuilder::build_header() {
    std::ostringstream header;
    header << R"(
    <header>
        <!-- Inicio navegación -->
        <nav class="navbar navbar-expand-lg navbar-dark bg-dark">
            <div class="container">
                <a class="navbar-brand" href="#">CompraCR</a>
                <div class="navbar-nav mr-auto">
)";
    header << std::string(5*4, ' ') << R"(<a class="nav-item nav-link" href="/">Inicio</a>)" << std::endl;
    if (logged) {
        header << std::string(5*4, ' ') << view::NavItem("/product/add", "Vender") << std::endl;
        header << std::string(5*4, ' ') << view::NavItem("/cart/checkout", "Carrito de compras") << std::endl;
        header << std::string(5*4, ' ') << view::NavItem("/user/logout", "Logout") << std::endl;
    } else {
        header << std::string(5*4, ' ') << view::NavItem("/user/add", "Registrarse") << std::endl;
        header << std::string(5*4, ' ') << view::NavItem("/user/login", "Login") << std::endl;
    }
    header << R"(
                </div>
)";
    if (logged) {
        header << R"(
            <div class="col-lg-2 text-white text-right">
                <div>Usuario(a):</div>
                <div>Artículos en carrito:</div>
            </div>
            <div class="col-lg-2 text-white">
                <div><strong>)" << username << R"(</strong></div>
                <div><strong>)" << items << R"(</strong></div>
            </div>
)";
    }
    header << R"(
            </div>
        </nav>
        <!-- Fin navegación -->
    </header>
)";
    return header.str();
}

std::string view::PageBuilder::build_body() {
    std::ostringstream body;

    body << R"(
<!-- Inicio contenido -->
        <main>
        <div class="container">
)";
    body << R"(
            <!-- Inicio del cuerpo de la página -->
            <div class="row bg-white page-title">
                <div class="col">
                    <div class="my-3"></div>
                    <div class="row">
                        <div class="col">
                            <h1 class="my-3" style="font-family: 'Alfa Slab One', cursive;">
)";
    body << title << R"(
                            </h1>
                        </div>
                    </div>)";
    body << build_content();
    body << R"(
                    <div class="my-5"></div>
                </div>
            </div>
            <!-- Fin del cuerpo de la página -->

        </div>
    </main>
    <!-- Fin contenido -->
)";

    return body.str();
}
/*
 *

 */

std::string view::PageBuilder::build_content() {
    std::ostringstream content;

    content << R"(
            <!-- Inicio Ofertas/últimos artículos -->
            <div class="row bg-white latest-items">
                <div class="col">
                    <div class="my-3"></div>
                    <div class="row">
                        <div class="col">
                            <h1 class="my-3" style="font-family: 'Alfa Slab One', cursive;">Ofertas</h1>
                        </div>
                    </div>)";
    //////////////
    // DECK ROW //
    content << R"(
                    <div class="row">
                        <!-- Inicio del card deck-->
                        <div class="card-deck mx-3 my-3">
)";
    content << ProductCard(0, "Computadora Dell",
                           "Procesador Intel i3, 4 GB de RAM y 126 GB de disco duro.", "₡ 250.000,00");
    content << ProductCard(0, "Motodguadaña Husqvarna", "Usada por 3 meses, en excelentes condiciones",
                           "₡ 58.000,00 ");
    content << ProductCard();
    content << R"(
                        </div>
                        <!-- Fin del card deck   -->
                    </div>
)";
    //          //
    //////////////

    //////////////
    // DECK ROW //
    content << R"(
                    <div class="row">
                        <!-- Inicio del card deck-->
                        <div class="card-deck mx-3 my-3">
)";
    content << ProductCard(0, "Computadora Dell",
                           "Procesador Intel i3, 4 GB de RAM y 126 GB de disco duro.", "₡ 250.000,00");
    content << ProductCard(0, "Motodguadaña Husqvarna", "Usada por 3 meses, en excelentes condiciones",
                           "₡ 58.000,00 ");
    content << ProductCard();
    content << R"(
                        </div>
                        <!-- Fin del card deck   -->
                    </div>
)";
    //          //
    //////////////

    content << R"(

                    <div class="my-5"></div>
                </div>
            </div>
            <!-- Fin Ofertas/últimos artículos -->
)";
    return content.str();
}

std::string view::PageBuilder::build_footer() {
    return R"(
    <!-- Inicio footer -->
    <footer>
        <div class="container-fluid bg-dark">
            <div class="container">
                <div class="row">
                    <div class="col align-middle">
                        <p class="text-white text-center py-2">
                            © Daniel Díaz Molina 2019
                        </p>
                    </div>
                </div>
            </div>
        </div>
    </footer>
    <!-- Fin footer -->
)";
}

view::PageBuilder::PageBuilder(const std::string &title) : title(title), logged(false) {}

void view::PageBuilder::setUserInfo(const std::string &name, const int &cartItems) {
    logged = true;
    username = name;
    items = cartItems;

}

view::PageBuilder::PageBuilder() : logged(false) {

}


