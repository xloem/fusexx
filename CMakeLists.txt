cmake_minimum_required(VERSION 3.1.2)

project(fuse++ CXX)

include(FindPkgConfig)
pkg_search_module(FUSE REQUIRED IMPORTED_TARGET fuse3 fuse)

add_library(fuse++ src/fuse++.cpp include/fuse++)
target_link_libraries(fuse++ PkgConfig::FUSE)

install(TARGETS fuse++)

#add_subdirectory(example)

add_executable(test test.cpp)
target_link_libraries(test fuse++)
