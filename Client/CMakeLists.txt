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

if(EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
	include("${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")

	conan_basic_setup(TARGETS)

	target_include_directories(babel_client PUBLIC
		${CONAN_INCLUDE_DIRS_QT}
		${CONAN_INCLUDE_DIRS_PORTAUDIO}
		${CONAN_INCLUDE_DIRS_OPUS}
	)

	conan_target_link_libraries(babel_client
								CONAN_PKG::QT
								CONAN_PKG::PORTAUDIO
								CONAN_PKG::OPUS
	)
#else()
#	message(SEND_ERROR " conanbuildinfo.cmake not found, you must call conan install to create it")
#endif()

find_package(qt)
find_package(portaudio)
find_package(opus)

target_link_libraries(babel_client qt::qt portaudio::portaudio opus::opus)

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
	set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/lib/catch2")
	find_package(Catch2 REQUIRED)
endif ()

target_link_libraries(client_unit_tests PRIVATE Catch2::Catch2)