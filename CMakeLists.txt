cmake_minimum_required(VERSION 3.9)
project(circular_buffer)

set(CMAKE_CXX_STANDARD 11)

add_executable(circular_buffer main.cpp circular_buffer.h test.cpp)