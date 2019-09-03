cmake_minimum_required(VERSION 3.14)
project(app_ecommerce C)

set(CMAKE_C_STANDARD 99)

add_executable(app_ecommerce src/main.c src/util/log.c src/util/log.h)