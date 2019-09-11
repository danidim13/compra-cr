//
// Created by daniel on 10/09/19.
//

#include <sstream>
#include "PageBuilder.h"

std::string PageBuilder::build_document() {
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

std::string PageBuilder::build_head() {
    return R"(
<head>
    <meta charset="UTF-8">
    <link rel="stylesheet" type="text/css" href="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css"/>
    <link href="https://fonts.googleapis.com/css?family=Alfa+Slab+One|Righteous&display=swap" rel="stylesheet">
    <title>CompraCR</title>
</head>
)";
}

std::string PageBuilder::build_header() {
    return R"(
    <header>
        <!-- Inicio navegación -->
        <nav class="navbar navbar-expand-lg navbar-dark bg-dark">
            <div class="container">
                <a class="navbar-brand" href="#">CompraCR</a>
                <div class="navbar-nav mr-auto">
                    <a class="nav-item nav-link ${ACTIVE = HOME}" href="#">Inicio</a>
                    <a class="nav-item nav-link ${ACTIVE = SELL}" href="#">Vender</a>
                    <a class="nav-item nav-link ${ACTIVE = CART}" href="#">Carrito de compras</a>
                </div>
            </div>
        </nav>
        <!-- Fin navegación -->
    </header>
)";
}

std::string PageBuilder::build_body() {
    return R"(
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
                    <form action="search" method="GET" role="search">
                        <div class="form-group row">
                            <div class="col-10">
                                <input type="text" class="form-control" placeholder="Búsqueda de productos">
                            </div>
                            <div class="col-2">
                                <button type="submit" class="btn btn-primary btn-block">Buscar</button>
                            </div>
                        </div>
                    </form>
                </div>
            </div>
            <!-- Fin barra de búsqueda -->

            <!-- Inicio Ofertas/últimos artículos -->
            <div class="row bg-white latest-items">
                <div class="col">
                    <div class="my-3"></div>
                    <div class="row">
                        <div class="col">
                            <h1 class="my-3" style="font-family: 'Alfa Slab One', cursive;">Ofertas</h1>
                        </div>
                    </div>

                    <div class="row">
                        <!-- Inicio del card deck-->
                        <div class="card-deck mx-3">

                            <div class="card">
                                <img src="https://via.placeholder.com/350x250" class="card-img-top" alt="imagen.jpg">
                                <div class="card-body">
                                    <h4 class="card-title">
                                        Motoguadaña Husqvarna
                                    </h4>
                                    <p class="card-text">Usada por 3 meses, en excelentes condiciones</p>
                                </div>
                                <div class="card-body">
                                    <p class="card-text">
                                        ₡ 58.000,00
                                    </p>
                                </div>
                            </div>
                            <div class="card">
                                <img src="https://via.placeholder.com/350x250" class="card-img-top" alt="imagen.jpg">
                                <div class="card-body">
                                    <h4 class="card-title">
                                        Motoguadaña Husqvarna
                                    </h4>
                                    <p class="card-text">Usada por 3 meses, en excelentes condiciones</p>
                                </div>
                                <div class="card-body">
                                    <p class="card-text">
                                        ₡ 58.000,00
                                    </p>
                                </div>
                            </div>
                            <div class="card">
                                <img src="https://via.placeholder.com/350x250" class="card-img-top" alt="imagen.jpg">
                                <div class="card-body">
                                    <h4 class="card-title">
                                        Motoguadaña Husqvarna
                                    </h4>
                                    <p class="card-text">XYX</p>
                                </div>
                                <div class="card-body">
                                    <p class="card-text">
                                        ₡ 58.000,00
                                    </p>
                                </div>
                            </div>
                        </div>
                        <!-- Fin del card deck   -->
                    </div>
                    <div class="my-5"></div>
                </div>
            </div>
            <!-- Fin Ofertas/últimos artículos -->

        </div>
    </main>
    <!-- Fin contenido -->
)";
}

std::string PageBuilder::build_footer() {
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
