
set(core_SRCS util/log.cpp util/log.h http/http.cpp http/http.h http/Router.cpp http/Router.h view/PageBuilder.cpp view/PageBuilder.h view/DomItem.cpp view/DomItem.h)

add_library(core ${core_SRCS})
add_executable(app_ecommerce.cgi main.cpp)
target_link_libraries(app_ecommerce.cgi -static-libstdc++ -static-libgcc core)
