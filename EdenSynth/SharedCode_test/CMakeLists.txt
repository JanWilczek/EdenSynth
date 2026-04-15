project(SharedCode_test)

set(gtest_force_shared_crt
    ON
    CACHE BOOL "")
find_package(GTest)

set(SOURCES
    source/viewmodels_test/PresetsViewModelTest.cpp
    source/ProductionPresetManagerTest.cpp
    source/PluginProcessorTest.cpp
    source/parameters_test/ParametersTest.cpp)

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

target_link_libraries(${PROJECT_NAME} PRIVATE GTest::gtest_main EdenSynth)

# Re-export compile definitions to ensure that
# the test target linking to EdenSynth will build JUCE modules with the identical options.
target_compile_definitions(SharedCode_test PRIVATE $<TARGET_PROPERTY:EdenSynth,COMPILE_DEFINITIONS>)

source_group(
  TREE ${CMAKE_CURRENT_SOURCE_DIR}/source
  PREFIX "Source Files"
  FILES ${SOURCES})

enable_testing()
include(GoogleTest)

gtest_discover_tests(${PROJECT_NAME})
