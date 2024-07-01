cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(SharedCode_test)

set(gtest_force_shared_crt
    ON
    CACHE BOOL "")
find_package(gtest CONFIG REQUIRED)

set(SOURCES source/viewmodels_test/PresetsViewModelTest.cpp source/ProductionPresetManagerTest.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
  target_compile_definitions(${PROJECT_NAME} PRIVATE _SILENCE_CXX23_ALIGNED_STORAGE_DEPRECATION_WARNING)
else()
  target_compile_options(
    ${PROJECT_NAME}
    PRIVATE -Wall
            -Wextra
            -pedantic
            -Werror)
endif()

get_target_property(EDEN_SYNTH_INCLUDE_DIRS EdenSynth INCLUDE_DIRECTORIES)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../SharedCode/include
                                                   ${EDEN_SYNTH_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PRIVATE GTest::gtest_main EdenSynth libeden)

source_group(
  TREE ${CMAKE_CURRENT_SOURCE_DIR}/source
  PREFIX "Source Files"
  FILES ${SOURCES})

enable_testing()
include(GoogleTest)

gtest_discover_tests(${PROJECT_NAME})

# Copy assets upon build
set(ASSETS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../assets)

add_custom_command(
  TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${ASSETS_PATH}" "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/assets/"
  COMMENT "Copy assets folder to test folder")
