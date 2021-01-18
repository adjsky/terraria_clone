cmake_minimum_required(VERSION 3.3)
project(terraria_clone)

set(CMAKE_CXX_STANDARD 17)
find_package(SFML 2.5.1 COMPONENTS system window graphics REQUIRED)
find_package(TGUI REQUIRED)

file(GLOB_RECURSE Sources Source/*.cpp)

add_executable(terraria_clone ${Sources} Source/Util/constants.h Source/Util/FastNoiseLite.h)
target_link_libraries(terraria_clone sfml-system sfml-window sfml-graphics tgui)