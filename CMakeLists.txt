cmake_minimum_required(VERSION 3.11)
project(Babel)

# COMMON_SOURCES is used by subdirectories
file(
	GLOB_RECURSE
	COMMON_SOURCES
	"${PROJECT_SOURCE_DIR}/Common/*.cpp"
	"${PROJECT_SOURCE_DIR}/Common/*.hpp"
)

set(CONAN_DISABLE_CHECK_COMPILER 1)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(KEEP_RPATHS)

if (EXISTS "${CMAKE_BINARY_DIR}/conan_paths.cmake")
	include("${CMAKE_BINARY_DIR}/conan_paths.cmake")
else()
	message(WARNING "The file conan_paths.cmake doesn't exist, you may have to run conan install first")
	list(APPEND CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR}")
endif()

include_directories("Common")

add_subdirectory("Client")
add_subdirectory("Server")

add_custom_target(unit_tests)
add_dependencies(unit_tests client_unit_tests server_unit_tests)