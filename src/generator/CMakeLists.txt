cmake_minimum_required(VERSION 3.5)

# Project Name
PROJECT(generator)

add_executable(generator
	matrix.cpp
	matrix.h
	generator.cpp
)

target_link_libraries(
	generator
)