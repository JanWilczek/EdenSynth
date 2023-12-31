cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(SharedCode_test)

set(CMAKE_CXX_STANDARD 23)

set(gtest_force_shared_crt ON CACHE BOOL "")
find_package(gtest REQUIRED PATHS ../dependencies)

set(SOURCES
    source/viewmodels_test/PresetsViewModelTest.cpp
    source/ProductionPresetManagerTest.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _SILENCE_CXX23_ALIGNED_STORAGE_DEPRECATION_WARNING)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../SharedCode/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../JuceLibraryCode
    ${CMAKE_CURRENT_SOURCE_DIR}/../JuceLibraryCode/modules
    ${CMAKE_CURRENT_SOURCE_DIR}/../libeden/include
)

add_library(EdenSynth STATIC IMPORTED)
# TODO: Change "Debug" folder to non-MSVC specific directory.
# Should be fixed when the whole project has migrated to CMake.
set_target_properties(EdenSynth PROPERTIES IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/../x64/Debug/Shared Code/EdenSynth.lib")
add_library(eden STATIC IMPORTED)
set_target_properties(eden PROPERTIES IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/../x64/Debug/libeden_x64_Debug.lib")

target_link_libraries(${PROJECT_NAME} PRIVATE
    GTest::gtest_main
    EdenSynth
    eden
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/source PREFIX "Source Files" FILES ${SOURCES})

enable_testing()
include(GoogleTest)

gtest_discover_tests(${PROJECT_NAME})
