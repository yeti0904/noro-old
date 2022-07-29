cmake_minimum_required(VERSION 2.9)
project(noro)

execute_process(COMMAND cp cmrc/CMakeRC.cmake .)
include(CMakeRC.cmake)

file(GLOB noro_src CONFIGURE_DEPENDS "src/*.cc")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-g -Wall -Wextra -pedantic")

cmrc_add_resource_library(
	noro-resources ALIAS noro::rc NAMESPACE noro
	runtime/settings.ini
	runtime/themes/dark16.ini
	runtime/themes/dark.ini
	runtime/themes/gruvy.ini
	runtime/themes/mono.ini
	runtime/themes/noro.ini
)

add_executable(noro ${noro_src})
target_link_libraries(noro PUBLIC ncurses PRIVATE noro::rc)

install(TARGETS noro DESTINATION "/usr/bin/")
