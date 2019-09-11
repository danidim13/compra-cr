cmake_minimum_required(VERSION 3.14)
project(app_ecommerce CXX)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(src)
add_subdirectory(Google_tests)



install(TARGETS app_ecommerce.cgi DESTINATION /opt/lampp/htdocs/ProyectoSeguridad)
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/webroot DESTINATION /opt/lampp/htdocs/ProyectoSeguridad)
install(FILES .htaccess DESTINATION /opt/lampp/htdocs/ProyectoSeguridad)
