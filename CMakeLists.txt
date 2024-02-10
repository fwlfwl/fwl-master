cmake_minimum_required(VERSION 2.8)
project(fwl)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O0 -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function")

include_directories(.)

include_directories(/usr/local/yaml.cpp-0.8.0/include)
link_directories(/usr/local/yaml.cpp-0.8.0/lib)

set(LIB_SRC
	fwl/http/ragel/httpclient_parser.cc
    fwl/http/ragel/http11_parser.cc
	fwl/http/http_parser.cc
	fwl/http/http.cc
	fwl/socket.cc
	fwl/address.cc
	fwl/endian.h
	fwl/fd_manager.cc
	fwl/bytearray.cc
	fwl/hook.cc
	fwl/timer.cc
    fwl/iomanager.cc
    fwl/scheduler.cc
    fwl/fiber.cc
    fwl/log.cc
    fwl/unit.cc
    fwl/config.cc
    fwl/thread.cc
    fwl/mutex.cc
    fwl/unit.cc
    )

add_library(fwl SHARED ${LIB_SRC})

add_executable(test_log tests/test_log.cc)
add_dependencies(test_log fwl)
target_link_libraries(test_log fwl yaml-cpp pthread )

add_executable(test_config tests/test_config.cc)
add_dependencies(test_config fwl)
target_link_libraries(test_config fwl yaml-cpp pthread )

add_executable(test_yaml tests/test_yaml.cc)
add_dependencies(test_yaml fwl)
target_link_libraries(test_yaml fwl yaml-cpp pthread )

add_executable(test_thread tests/test_thread.cc)
add_dependencies(test_thread fwl)
target_link_libraries(test_thread fwl yaml-cpp pthread )

add_executable(test_unit tests/test_unit.cc)
add_dependencies(test_unit fwl)
target_link_libraries(test_unit fwl yaml-cpp pthread )

add_executable(test_fiber tests/test_fiber.cc)
add_dependencies(test_fiber fwl)
target_link_libraries(test_fiber fwl yaml-cpp pthread)

add_executable(test_scheduler tests/test_scheduler.cc)
add_dependencies(test_scheduler fwl)
target_link_libraries(test_scheduler fwl yaml-cpp pthread)

add_executable(test_iomanager tests/test_iomanager.cc)
add_dependencies(test_iomanager fwl)
target_link_libraries(test_iomanager fwl yaml-cpp pthread)

add_executable(test_hook tests/test_hook.cc)
add_dependencies(test_hook fwl)
target_link_libraries(test_hook fwl yaml-cpp pthread)

add_executable(test_bytearray tests/test_bytearray.cc)
add_dependencies(test_bytearray fwl)
target_link_libraries(test_bytearray fwl yaml-cpp pthread)

add_executable(test_address tests/test_address.cc)
add_dependencies(test_address fwl)
target_link_libraries(test_address fwl yaml-cpp pthread)

add_executable(test_socket tests/test_socket.cc)
add_dependencies(test_socket fwl)
target_link_libraries(test_socket fwl yaml-cpp pthread)

add_executable(test_http tests/test_http.cc)
add_dependencies(test_http fwl)
target_link_libraries(test_http fwl yaml-cpp pthread)

add_executable(test_http_parser tests/test_http_parser.cc)
add_dependencies(test_http_parser fwl)
target_link_libraries(test_http_parser fwl yaml-cpp pthread)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)



