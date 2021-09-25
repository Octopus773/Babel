cmake_minimum_required(VERSION 3.11)
project(Babel_Client)

set(CMAKE_CXX_STANDARD 20)

if (CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -Wshadow -W -g")
endif ()
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

file(
	GLOB_RECURSE
	SOURCES
	"${PROJECT_SOURCE_DIR}/src/*.cpp"
	"${PROJECT_SOURCE_DIR}/src/*.hpp"
)

list(REMOVE_ITEM SOURCES "${PROJECT_SOURCE_DIR}/src/main.cpp")

include_directories(src)

add_executable(babel_client
	"${PROJECT_SOURCE_DIR}/src/main.cpp"
	${SOURCES}
	${COMMON_SOURCES}
)

find_package(Qt5 5.15.2 REQUIRED COMPONENTS Widgets)
find_package(portaudio 19.7.0 REQUIRED)
find_package(Opus 1.3.1 REQUIRED)

target_link_libraries(babel_client Qt5::Widgets portaudio::portaudio Opus::opus)

file(
	GLOB_RECURSE
	TESTS_SOURCES
	"${PROJECT_SOURCE_DIR}/tests/*.cpp"
	"${PROJECT_SOURCE_DIR}/tests/*.hpp"
)

add_executable(client_unit_tests EXCLUDE_FROM_ALL
	${TESTS_SOURCES}
	${SOURCES}
	${COMMON_SOURCES}
)

find_package(Catch2 QUIET)
if (NOT Catch2_FOUND)
	list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/lib/catch2")
	find_package(Catch2 REQUIRED)
endif ()

target_link_libraries(client_unit_tests PRIVATE Catch2::Catch2)