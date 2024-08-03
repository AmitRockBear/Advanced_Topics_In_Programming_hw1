cmake_minimum_required(VERSION 3.29)
project(HW3 CXX)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-std=c++20 -Wall -Wextra -Werror -pedantic -fsanitize=address -g)
add_link_options(-fsanitize=address)

add_subdirectory(simulator)
add_subdirectory(algorithms/deterministic)
add_subdirectory(algorithms/random)