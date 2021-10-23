cmake_minimum_required (VERSION 3.8)

project ("amogus-skins")

file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.c" "${CMAKE_SOURCE_DIR}/lib/*.c")

include_directories("${CMAKE_SOURCE_DIR}/lib/minhook/include")

add_library(amogus-skins SHARED ${SOURCES})