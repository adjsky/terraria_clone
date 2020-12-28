cmake_minimum_required(VERSION 3.17)
project(terraria_clone)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(Game)
add_subdirectory(InputHandler)

add_executable(terraria_clone main.cpp constants.h)
target_link_libraries(terraria_clone sfml-window sfml-system sfml-graphics game input-handler)