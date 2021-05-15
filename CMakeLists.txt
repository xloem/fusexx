cmake_minimum_required(VERSION 3.1.2)

project(fuse++ C++)

add_library(fuse++ src/fuse++.cpp include/fuse++)
install(TARGET fuse++)

add_executable(test test.cpp)
target_link_libraries(test fuse++)
