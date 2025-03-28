cmake_minimum_required(VERSION 3.10)
project(MotorOpenGL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(assimp REQUIRED)
find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})
link_directories(${FREETYPE_LIBRARY_DIRS})

add_executable(MotorOpenGL 
    src/main.cpp
    src/Shader.cpp
    src/Textura.cpp
    src/Graphics2D.cpp
    src/Graphics3D.cpp
    src/glad.c
    src/arbol.cpp
    src/entidad.cpp
    src/gestorRecursos.cpp
    src/motortag.cpp
)

target_include_directories(MotorOpenGL PUBLIC include)
target_link_libraries(MotorOpenGL PRIVATE glfw OpenGL::GL assimp ${FREETYPE_LIBRARIES})
