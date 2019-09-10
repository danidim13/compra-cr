cmake_minimum_required(VERSION 3.14)
project(app_ecommerce CXX)

set(CMAKE_CXX_STANDARD 11)

add_executable(app_ecommerce.cgi src/main.cpp src/util/log.cpp src/util/log.h src/http/http.cpp src/http/http.h src/http/Router.cpp src/http/Router.h)
target_link_libraries(app_ecommerce.cgi -static-libstdc++ -static-libgcc)

install(TARGETS app_ecommerce.cgi DESTINATION /opt/lampp/htdocs/ProyectoSeguridad)
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/webroot DESTINATION /opt/lampp/htdocs/ProyectoSeguridad)
install(FILES .htaccess DESTINATION /opt/lampp/htdocs/ProyectoSeguridad)
