cmake_minimum_required(VERSION 3.14)
project(app_ecommerce CXX)

set(CMAKE_CXX_STANDARD 11)

add_executable(app_ecommerce src/main.cpp src/util/log.cpp src/util/log.h src/http/http.cpp src/http/http.h src/http/Router.cpp src/http/Router.h)
target_link_libraries(app_ecommerce -static-libstdc++ -static-libgcc)

install(TARGETS app_ecommerce DESTINATION /opt/lampp/cgi-bin)