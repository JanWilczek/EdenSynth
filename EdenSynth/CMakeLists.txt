cmake_minimum_required(VERSION 3.23 FATAL_ERROR)

project(eden)

enable_testing()

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(libeden)
add_subdirectory(libeden_test)

add_subdirectory(SharedCode)
add_subdirectory(SharedCode_test)

set_directory_properties(PROPERTIES VS_STARTUP_PROJECT EdenSynth_Standalone)
