cmake_minimum_required(VERSION 3.19)
project(Babel)

set(CMAKE_CXX_STANDARD 20)


if (CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -Wshadow -W -g")
endif ()
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(SOURCES_CLIENT
	src/main.cpp
	)

set(SOURCES_SERVER
	src/main.cpp
	)

include_directories(src)

add_executable(babel_client
	${SOURCES_CLIENT}
	)

add_executable(babel_server
	${SOURCES_SERVER}
	)

add_executable(unit_tests EXCLUDE_FROM_ALL
	${SOURCES_CLIENT}
	${SOURCES_SERVER}
	tests/Catch2Main.cpp
	)

find_package(Catch2 QUIET)
if (NOT Catch2_FOUND)
	set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/lib/catch2)
	find_package(Catch2 REQUIRED)
endif ()
target_link_libraries(unit_tests PRIVATE Catch2::Catch2)