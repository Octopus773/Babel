cmake_minimum_required(VERSION 3.11)
project(Babel_Client LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

file(
	GLOB_RECURSE
	SOURCES
	"${PROJECT_SOURCE_DIR}/src/*.cpp"
	"${PROJECT_SOURCE_DIR}/src/*.hpp"
)

list(REMOVE_ITEM SOURCES "${PROJECT_SOURCE_DIR}/src/main.cpp")

include_directories("src")

add_executable(babel_server
	"${PROJECT_SOURCE_DIR}/src/main.cpp"
	${SOURCES}
	${COMMON_SOURCES}
)


find_package(Opus 1.3.1 REQUIRED)
find_package(asio 1.19.2 REQUIRED)

target_link_libraries(babel_server asio::asio Opus::opus)

if (UNIX)
	target_compile_options(babel_server PUBLIC
		-fPIC -Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wuninitialized -Wunreachable-code
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
target_link_libraries(server_unit_tests PRIVATE Catch2::Catch2)