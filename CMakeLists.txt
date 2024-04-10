cmake_minimum_required(VERSION 2.8)
project(fwl)

include(cmake/unit.cmake)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -ggdb -std=c++20 -lcrypto -Wall -Wno-deprecated -Werror -Wno-unused-function")

include_directories(.)

include_directories(/usr/local/yaml.cpp-0.8.0/include)
link_directories(/usr/local/yaml.cpp-0.8.0/lib)

set(LIB_SRC
	fwl/uri.cc
	fwl/sock_stream.cc
	fwl/stream.cc
	fwl/network_server.cc
	fwl/http/ws_session.cc
	fwl/http/ws_servlet.cc
	fwl/http/ws_server.cc
	fwl/http/http_connection.cc
	fwl/http/http_servlet.cc
	fwl/http/http_server.cc
	fwl/http/http_session.cc
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

find_package(OpenSSL REQUIRED)

set (LIBS 
	fwl 
	yaml-cpp
	pthread
	${OPENSSL_LIBRARIES}
)

fwl_add_executable(test_log "tests/test_log.cc" fwl "${LIBS}")

fwl_add_executable(test_config "tests/test_config.cc" fwl "${LIBS}")

fwl_add_executable(test_yaml "tests/test_yaml.cc" fwl "${LIBS}")

fwl_add_executable(test_thread "tests/test_thread.cc" fwl "${LIBS}")

fwl_add_executable(test_unit "tests/test_unit.cc" fwl "${LIBS}")

fwl_add_executable(test_fiber "tests/test_fiber.cc" fwl "${LIBS}")

fwl_add_executable(test_scheduler "tests/test_scheduler.cc" fwl "${LIBS}")

fwl_add_executable(test_iomanager "tests/test_iomanager.cc" fwl "${LIBS}")

fwl_add_executable(test_hook "tests/test_hook.cc" fwl "${LIBS}")

fwl_add_executable(test_bytearray "tests/test_bytearray.cc" fwl "${LIBS}")

fwl_add_executable(test_address "tests/test_address.cc" fwl "${LIBS}")

fwl_add_executable(test_socket "tests/test_socket.cc" fwl "${LIBS}")

fwl_add_executable(test_http "tests/test_http.cc" fwl "${LIBS}")

fwl_add_executable(test_http_parser "tests/test_http_parser.cc" fwl "${LIBS}")

fwl_add_executable(test_networkserver "tests/test_networkserver.cc" fwl "${LIBS}")

fwl_add_executable(test_server "tests/test_server.cc" fwl "${LIBS}")

fwl_add_executable(test_uri "tests/test_uri.cc" fwl "${LIBS}")

fwl_add_executable(test_http_connection "tests/test_http_connection.cc" fwl "${LIBS}")

fwl_add_executable(test_wsServer "tests/test_wsServer.cc" fwl "${LIBS}")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)



