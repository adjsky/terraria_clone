cmake_minimum_required(VERSION 3.3)
project(terraria_clone)

set(CMAKE_CXX_STANDARD 17)
find_package(SFML 2.5.1 COMPONENTS system window graphics REQUIRED)
find_package(TGUI REQUIRED)
find_package(Boost 1.75 COMPONENTS serialization REQUIRED)

file(GLOB_RECURSE Sources Source/*.cpp)

add_executable(terraria_clone ${Sources})
target_link_libraries(terraria_clone PRIVATE sfml-system sfml-window sfml-graphics tgui Boost::serialization)
#target_link_options(terraria_clone PRIVATE -mwindows)