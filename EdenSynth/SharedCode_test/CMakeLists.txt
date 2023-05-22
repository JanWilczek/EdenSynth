cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(SharedCode_test)

set(CMAKE_CXX_STANDARD 23)

set(gtest_force_shared_crt ON CACHE BOOL "")
find_package(gtest REQUIRED PATHS ../dependencies)

add_executable(${PROJECT_NAME} source/viewmodels_test/PresetsViewModelTest.cpp)
target_link_libraries(${PROJECT_NAME}
    GTest::gtest_main
)

enable_testing()
include(GoogleTest)

gtest_discover_tests(${PROJECT_NAME})
