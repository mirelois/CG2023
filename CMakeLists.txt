cmake_minimum_required(VERSION 3.5)

# Project Name
PROJECT(CG2023)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(OpenGL_GL_PREFERENCE "GLVND")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -pedantic")

add_subdirectory(src/generator)
add_subdirectory(src/engine)

target_include_directories(generator PUBLIC include)
target_include_directories(engine PUBLIC include)







