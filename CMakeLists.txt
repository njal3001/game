cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)
project(game)

find_package(SDL2 REQUIRED)

include_directories(include ${SDL2_INCLUDE_DIRS})

add_executable(game src/main.cpp)

target_link_libraries(game ${SDL2_LIBRARIES})
