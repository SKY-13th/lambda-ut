cmake_minimum_required(VERSION 3.8)
set (CMAKE_CXX_FLAGS "-std=gnu++11 ${CMAKE_CXX_FLAGS}")
add_executable(example example.cpp lambda-ut.h)
