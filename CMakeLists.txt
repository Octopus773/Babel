cmake_minimum_required(VERSION 3.19)
project(Babel)

add_subdirectory(Client)
add_subdirectory(Server)

add_custom_target(unit_tests)
add_dependencies(unit_tests client_unit_tests server_unit_tests)