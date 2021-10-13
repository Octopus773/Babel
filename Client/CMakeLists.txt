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

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include_directories(src)

add_executable(babel_client
	"${PROJECT_SOURCE_DIR}/src/main.cpp"
	${SOURCES}
	${COMMON_SOURCES}
)

find_package(Qt5 COMPONENTS Widgets Network Gui Core REQUIRED)
find_package(portaudio 19.7.0 REQUIRED)
find_package(Opus 1.3.1 REQUIRED)

target_link_libraries(babel_client Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network portaudio::portaudio Opus::opus)

if (UNIX)
	target_compile_options(babel_client PUBLIC
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

target_link_libraries(client_unit_tests PRIVATE Catch2::Catch2 Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network  portaudio::portaudio Opus::opus)