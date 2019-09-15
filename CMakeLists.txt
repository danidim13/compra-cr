cmake_minimum_required(VERSION 3.10)
project(app_ecommerce CXX)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(src)
add_subdirectory(Google_tests EXCLUDE_FROM_ALL)

#set(HTDOCS /opt/lampp/htdocs/ProyectoSeguridad)
set(HTDOCS /var/www/compra.cr)



install(TARGETS app_ecommerce.cgi DESTINATION ${HTDOCS})
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/webroot DESTINATION ${HTDOCS})
install(FILES .htaccess DESTINATION ${HTDOCS})
