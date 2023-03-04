cmake_minimum_required(VERSION 3.5)

add_executable(generator
	matrix.cpp
	matrix.h
	generator.cpp
)

target_link_libraries(
	generator
)