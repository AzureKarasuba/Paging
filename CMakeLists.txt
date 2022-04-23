cmake_minimum_required(VERSION 3.22)
project(Paging)

set(CMAKE_CXX_STANDARD 17)

add_executable(Paging main.cpp PageTable.cpp PageTable.h Page.cpp Page.h)
