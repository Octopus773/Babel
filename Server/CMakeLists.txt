cmake_minimum_required(VERSION 3.11)
project(Babel_Server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

file(
	GLOB_RECURSE
	SOURCES
	"${PROJECT_SOURCE_DIR}/src/*.cpp"
	"${PROJECT_SOURCE_DIR}/src/*.hpp"
)

list(REMOVE_ITEM SOURCES "${PROJECT_SOURCE_DIR}/src/main.cpp")
list(REMOVE_ITEM SOURCES "${PROJECT_SOURCE_DIR}/src/main2.cpp")

include_directories("src")

add_executable(babel_server
	"${PROJECT_SOURCE_DIR}/src/main.cpp"
	${SOURCES}
	${COMMON_SOURCES}
)

add_executable(babel_client_asio
	"${PROJECT_SOURCE_DIR}/src/main2.cpp"
	${SOURCES}
	${COMMON_SOURCES}
	)


find_package(asio 1.19.2 REQUIRED)

target_link_libraries(babel_server PRIVATE asio::asio)
target_link_libraries(babel_client_asio PRIVATE asio::asio)

if (UNIX)
	target_compile_options(babel_server PUBLIC
		-fPIC -Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wuninitialized -Wunreachable-code -fcoroutines
		$<$<CONFIG:RELEASE>:-Ofast>
		)
endif ()

file(
	GLOB_RECURSE
	TESTS_SOURCES
	"${PROJECT_SOURCE_DIR}/tests/*.cpp"
	"${PROJECT_SOURCE_DIR}/tests/*.hpp"
)

add_executable(server_unit_tests EXCLUDE_FROM_ALL
	${TESTS_SOURCES}
	${SOURCES}
	${COMMON_SOURCES}
	)

find_package(Catch2 QUIET)
if (NOT Catch2_FOUND)
	list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/lib/catch2")
	find_package(Catch2 REQUIRED)
endif ()
target_link_libraries(server_unit_tests PRIVATE Catch2::Catch2 asio::asio)