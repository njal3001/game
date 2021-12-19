cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)
project(game)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(Freetype REQUIRED)

include_directories(include ${SDL2_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS})

add_executable(sb 
    src/main.cpp 

    src/sb/game.cpp
    src/sb/collision.cpp

    src/engine/platform.cpp 
    src/engine/input.cpp 
    src/engine/log.cpp 

    src/engine/maths/vec2.cpp
    src/engine/maths/mat4x4.cpp
    src/engine/maths/mat3x3.cpp
    src/engine/maths/calc.cpp
    src/engine/maths/rect.cpp
    
    src/engine/graphics/graphics.cpp
    src/engine/graphics/color.cpp
    src/engine/graphics/renderer.cpp
    src/engine/graphics/renderer.cpp
    src/engine/graphics/texture.cpp
    src/engine/graphics/image.cpp
    src/engine/graphics/shader.cpp
    src/engine/graphics/subtexture.cpp
    src/engine/graphics/font.cpp
    )

target_link_libraries(sb ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${FREETYPE_LIBRARIES})
