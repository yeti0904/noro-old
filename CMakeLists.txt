cmake_minimum_required(VERSION 2.9)
project(noro)

file(GLOB noro_src CONFIGURE_DEPENDS "src/*.cc")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-g -Wall -Wextra -pedantic")

add_executable(noro ${noro_src})

target_link_libraries(noro ncurses)

install(TARGETS noro DESTINATION "/usr/bin/")
