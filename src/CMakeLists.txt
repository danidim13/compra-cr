
find_package(OpenSSL REQUIRED)

set(core_SRCS
        nlohmann/json.hpp
        nlohmann/json_fwd.hpp
        util/cookie.cpp util/cookie.h
        util/decode.h util/decode.cpp
        util/log.cpp util/log.h
        http/http.cpp http/http.h
        http/Router.cpp http/Router.h
        http/Request.cpp http/Request.h
        http/Controller.cpp http/Controller.h
        http/Response.cpp http/Response.h
        http/UserController.cpp http/UserController.h
        http/ProductController.cpp http/ProductController.h
        http/CartController.cpp http/CartController.h
        model/model.cpp model/model.h
        model/ConnectionHandler.h
        model/User.cpp model/User.h
        model/Product.cpp model/Product.h
        model/Purchase.cpp model/Purchase.h
        model/CardPayment.cpp model/CardPayment.h
        view/ProductCard.cpp view/ProductCard.h
        view/product/ProductListBuilder.cpp view/product/ProductListBuilder.h
        view/user/UserAddBuilder.cpp view/user/UserAddBuilder.h
        view/Form.cpp view/Form.h
        view/FormInput.cpp view/FormInput.h
        view/PageBuilder.cpp view/PageBuilder.h
        view/DomItem.cpp view/DomItem.h
        view/NavItem.cpp view/NavItem.h
        view/product/ProductAddBuilder.cpp view/product/ProductAddBuilder.h
        view/user/UserLoginBuilder.cpp view/user/UserLoginBuilder.h
        view/Table.cpp view/Table.h
        view/cart/CheckoutBuilder.cpp view/cart/CheckoutBuilder.h
        view/other/NotFoundPageBuilder.cpp view/other/NotFoundPageBuilder.h
        validate/Validator.cpp validate/Validator.h
        validate/EmailValidator.cpp validate/EmailValidator.h
        validate/StringValidator.cpp validate/StringValidator.h
        validate/MapValidator.cpp validate/MapValidator.h
        auth/PasswordHasher.cpp auth/PasswordHasher.h
        auth/SessionManager.cpp auth/SessionManager.h
        model/Session.cpp model/Session.h)


add_library(core ${core_SRCS})

find_path(MYSQLCPP_INCLUDE_DIR mysql_connection.h )

find_library(MYSQLCPP_LIBRARY libmysqlcppconn.so)
#find_library(MYSQLCPP_LIBRARY NAMES  libmysqlclient.a libmysqlcppconn-static.a)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

if(NOT MYSQLCPP_INCLUDE_DIR)
    message(${MYSQLCPP_INCLUDE_DIR})
    message(FATAL_ERROR "mysql_connection.h not found, try installing with apt-get install libmysqlcppconn-dev")
elseif(NOT MYSQLCPP_LIBRARY)
    message(${MYSQLCPP_LIBRARY})
    message(FATAL_ERROR "libmysqlcppconn.so not found, try installing with apt-get install libmysqlcppconn-dev")
else()
    message(STATUS "FOUND libmsqlcppconn")
endif()

target_include_directories(core PRIVATE ${MYSQLCPP_INCLUDE_DIR})
target_link_libraries(core PUBLIC Threads::Threads PRIVATE ${MYSQLCPP_LIBRARY} OpenSSL::Crypto)

# include(FindPkgConfig)
# pkg_check_modules(LIBMYSQLCLIENT REQUIRED mysqlclient)
#
# foreach(FLAG ${LIBMYSQLCLIENT_CFLAGS_OTHER})
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAG}")
# endforeach()
# link_directories(${LIBMYSQLCLIENT_LIBRARY_DIRS})
# target_include_directories(core PRIVATE ${LIBMYSQLCLIENT_INCLUDE_DIRS})
# target_link_libraries(core PRIVATE ${LIBMYSQLCLIENT_LIBRARIES})

add_executable(app_ecommerce.cgi main.cpp)
target_link_libraries(app_ecommerce.cgi -static-libstdc++ -static-libgcc core )

add_executable(manual_test test.cpp)
target_link_libraries(manual_test core)

install(TARGETS app_ecommerce.cgi DESTINATION ${HTDOCS})
