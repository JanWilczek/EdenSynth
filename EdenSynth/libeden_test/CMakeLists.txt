cmake_minimum_required(VERSION 3.22)

project(libeden_test)

enable_testing()

find_package(gtest CONFIG REQUIRED)

add_executable(
  libeden_test
  eden_test/AudioBufferTest.cpp
  eden_test/MidiBufferTest.cpp
  interpolation_test/LinearInterpolatorTest.cpp
  pch.h
  synth_test/envelope_test/ADBDRTest.cpp
  synth_test/envelope_test/EnvelopeTest.cpp
  synth_test/envelope_test/SegmentsTest.cpp
  synth_test/SynthesiserTest.cpp
  synth_test/VoiceTest.cpp
  synth_test/wavetable_test/SignalGeneratorTest.cpp
  synth_test/wavetable_test/WhiteNoiseSourceTest.cpp
  TestUtils.h
  utility_test/dsp_test/CorrelationTest.cpp
  utility_test/TimeSampleConverterTest.cpp)

target_precompile_headers(libeden_test PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/pch.h>")

target_link_libraries(libeden_test PRIVATE GTest::gtest_main libeden)

target_include_directories(libeden_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if(MSVC)
  target_compile_options(libeden_test PRIVATE /WX /W3)
endif()

include(GoogleTest)
gtest_discover_tests(libeden_test)
