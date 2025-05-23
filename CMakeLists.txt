cmake_minimum_required(VERSION 3.10)
project(MotorOpenGL)

# Establecer el estándar de C++ y C
set(CMAKE_CXX_STANDARD 23)  # Cambié C++17 a C++23
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_C_STANDARD 11)  # Establecer el estándar de C, por ejemplo C11
set(CMAKE_C_STANDARD_REQUIRED True)

# Establecer política para la versión de CMake
cmake_policy(SET CMP0072 NEW)

# Habilitar advertencias estrictas para los compiladores C y C++ (dependiendo de tu compilador, esto puede variar)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
elseif(MSVC)
    add_compile_options(/W4)
endif()

# Activar opciones de optimización (solo en modo Release)
set(CMAKE_BUILD_TYPE "Release")
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O3 -march=native -flto)
endif()

# Encontrar dependencias
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(assimp REQUIRED)
find_package(Freetype REQUIRED)

# Incluir y enlazar directorios necesarios para Freetype
include_directories(${FREETYPE_INCLUDE_DIRS})
link_directories(${FREETYPE_LIBRARY_DIRS})

# Agregar las opciones de depuración (si se está compilando en modo Debug)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g -O0)  # Añadir opciones de depuración
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG")  # Asegurar que se agregue la bandera de depuración
    set(CMAKE_C_FLAGS_DEBUG "-g -O0 -DDEBUG")  # Asegurar que se agregue la bandera de depuración para C
endif()

# Definir las optimizaciones y flags adicionales para C++
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -fvisibility=hidden")

# Definir las dependencias
add_executable(MotorOpenGL 
    src/arbol.cpp
    src/Camara.cpp
    src/entidad.cpp
    src/Fichero.cpp
    src/gestorRecursos.cpp
    src/glad.c
    src/Graphics2D.cpp
    src/Graphics3D.cpp
    src/Luz.cpp
    src/main.cpp
    src/Mesh.cpp
    src/motortag.cpp
    src/Shader.cpp
    src/Textura.cpp
    src/Window.cpp
    src/ParticleGenerator.cpp
    src/ParticleGenerator3D.cpp
    src/MeshAnimada.cpp
    src/ModelAnimation.cpp
    src/gestorEntidades.cpp
)

# Incluir directorios
target_include_directories(MotorOpenGL PUBLIC include)

# Enlazar con las librerías necesarias
target_link_libraries(MotorOpenGL PRIVATE glfw OpenGL::GL assimp ${FREETYPE_LIBRARIES})

# Habilitar soporte para C++23, puedes especificar el soporte para características adicionales si es necesario
set_target_properties(MotorOpenGL PROPERTIES CXX_STANDARD 23 CXX_STANDARD_REQUIRED ON)

# Agregar flags para cada plataforma de compilación
if(MSVC)
    # Para MSVC, podemos usar opciones como /MP para multihilo en compilación
    add_compile_options(/MP)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # Para GCC/Clang, podemos habilitar la optimización a nivel de enlace y la compilación de múltiples procesos
    add_compile_options(-flto -fopenmp)
endif()

# Desactivar -Wpedantic solo para glad.c
set_source_files_properties(src/glad.c PROPERTIES COMPILE_OPTIONS "-Wno-pedantic")

# Opciones adicionales para el compilador C
if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
elseif(MSVC)
    add_compile_options(/W4)
endif()

# Asegurarse de que solo C++ tenga la opción -std=c++23
target_compile_options(MotorOpenGL PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-std=c++23>)

