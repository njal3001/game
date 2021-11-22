cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)
project(game)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(include ${SDL2_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})

add_executable(game 
    src/main.cpp 
    src/platform.cpp 
    src/input.cpp 
    src/log.cpp 

    src/maths/vec2.cpp
    src/maths/mat4x4.cpp
    src/maths/mat3x3.cpp
    src/maths/calc.cpp

    src/graphics/graphics.cpp
    src/graphics/color.cpp
    src/graphics/renderer.cpp
    src/graphics/renderer.cpp
    src/graphics/texture.cpp
    src/graphics/image.cpp
    src/graphics/shader.cpp
    )

target_link_libraries(game ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})
