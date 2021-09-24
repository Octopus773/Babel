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

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

include_directories("Common")

add_subdirectory("Client")
add_subdirectory("Server")

add_custom_target(unit_tests)
add_dependencies(unit_tests client_unit_tests server_unit_tests)