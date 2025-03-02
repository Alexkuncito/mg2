cmake_minimum_required(VERSION 3.10)
project(MotorOpenGL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(assimp REQUIRED)

add_executable(MotorOpenGL 
    src/main.cpp
    src/Shader.cpp
    src/glad.c
    src/gestorRecursos.cpp
)

target_include_directories(MotorOpenGL PUBLIC include)
target_link_libraries(MotorOpenGL PRIVATE glfw OpenGL::GL assimp)
