cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(eden)

include_external_msproject(libeden ${CMAKE_CURRENT_SOURCE_DIR}/libeden/libeden.vcxproj)
add_subdirectory(SharedCode_test)
