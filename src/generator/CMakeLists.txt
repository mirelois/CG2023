cmake_minimum_required(VERSION 3.5)

# Project Name
PROJECT(generator)

add_executable(generator
	generator.cpp
	bezier.cpp
	bezier.h
)

target_link_libraries(
	generator
)
