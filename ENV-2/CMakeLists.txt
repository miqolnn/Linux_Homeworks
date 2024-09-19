cmake_minimum_required(VERSION 3.10)
project(CSort)

add_library(Complex STATIC Complex.cpp)
add_library(Sort STATIC Sort.cpp)

add_executable(csort main.cpp)
target_link_libraries(csort PRIVATE Complex Sort)
target_compile_options(csort PRIVATE -w)
target_compile_options(Complex PRIVATE -w)
target_compile_options(Sort PRIVATE -w)

add_custom_target(clean-csort
	COMMAND ${CMAKE_COMMAND} -E remove ./csort
)
