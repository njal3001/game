cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)
project(game)

find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)

include_directories(include ${SDL2_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})

add_executable(game src/main.cpp src/platform.cpp src/input.cpp src/log.cpp)

target_link_libraries(game ${SDL2_LIBRARIES} ${GLEW_LIBRARIES})
