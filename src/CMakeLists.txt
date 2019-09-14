
set(core_SRCS util/log.cpp util/log.h http/http.cpp http/http.h http/Router.cpp http/Router.h view/PageBuilder.cpp view/PageBuilder.h view/DomItem.cpp view/DomItem.h view/NavItem.cpp view/NavItem.h http/Request.cpp http/Request.h model/model.cpp model/model.h)

add_library(core ${core_SRCS})
add_executable(app_ecommerce.cgi main.cpp)
add_executable(manual_test test.cpp)
target_link_libraries(app_ecommerce.cgi -static-libstdc++ -static-libgcc core)
target_link_libraries(manual_test core)
