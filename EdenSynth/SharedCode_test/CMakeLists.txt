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

target_include_directories(
  ${PROJECT_NAME}
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../SharedCode/include
          ${CMAKE_CURRENT_SOURCE_DIR}/../JuceLibraryCode
          ${CMAKE_CURRENT_SOURCE_DIR}/../JuceLibraryCode/modules
          ${CMAKE_CURRENT_SOURCE_DIR}/../libeden/include)

add_library(EdenSynth_msvc STATIC IMPORTED)
set_target_properties(
  EdenSynth_msvc
  PROPERTIES IMPORTED_LOCATION_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/../x64/Debug/Shared Code/EdenSynth.lib"
             IMPORTED_LOCATION_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/../x64/Release/Shared Code/EdenSynth.lib"
             MAP_IMPORTED_CONFIG_MINSIZEREL Release
             MAP_IMPORTED_CONFIG_RELWITHDEBINFO Release)

target_link_libraries(${PROJECT_NAME} PRIVATE GTest::gtest_main EdenSynth_msvc libeden)

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
