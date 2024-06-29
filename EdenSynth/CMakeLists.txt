cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(eden)

include_external_msproject(libeden ${CMAKE_CURRENT_SOURCE_DIR}/libeden/libeden.vcxproj)
include_external_msproject(libeden_test ${CMAKE_CURRENT_SOURCE_DIR}/libeden_test/libeden_test.vcxproj libeden)
add_subdirectory(SharedCode_test)
