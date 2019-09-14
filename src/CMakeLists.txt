
set(core_SRCS
        util/log.cpp util/log.h
        http/http.cpp http/http.h
        http/Router.cpp http/Router.h
        view/PageBuilder.cpp view/PageBuilder.h
        view/DomItem.cpp view/DomItem.h
        view/NavItem.cpp view/NavItem.h
        http/Request.cpp http/Request.h
        model/model.cpp model/model.h
        model/ConectionHandler.cpp model/ConectionHandler.h
        model/User.cpp model/User.h
)


add_library(core ${core_SRCS})

find_path(MYSQLCPP_INCLUDE_DIR mysql_connections.h)
find_library(MYSQLCPP_LIBRARY libmysqlcppconn.so)

if((NOT MYSQLCPP_INCLUDE_DIR) OR (NOT MYSQLCPP_LIBRARY))
    message(FATAL_ERROR "libmysqlcppconn not found, try installing with apt-get install libmysqlcppconn-dev")
else()
    message(STATUS "FOUND libmsqlcppconn")
endif()

target_include_directories(core PRIVATE ${MYSQLCPP_INCLUDE_DIR})
target_link_libraries(core PRIVATE ${MYSQLCPP_LIBRARY})

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
