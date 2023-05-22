cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(SharedCode_test)

set(CMAKE_CXX_STANDARD 23)
if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

set(gtest_force_shared_crt ON CACHE BOOL "")
find_package(gtest REQUIRED PATHS ../dependencies)

add_executable(${PROJECT_NAME} source/viewmodels_test/PresetsViewModelTest.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE
    ../SharedCode/include
)

add_library(EdenSynth STATIC IMPORTED)
# TODO: Change "Debug" folder to non-MSVC specific directory.
# Should be fixed when the whole project has migrated to CMake.
set_target_properties(EdenSynth PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/../x64/Debug/Shared Code/EdenSynth.lib")
target_link_libraries(${PROJECT_NAME} PRIVATE
    GTest::gtest_main
    EdenSynth
)

enable_testing()
include(GoogleTest)

gtest_discover_tests(${PROJECT_NAME})
