cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(eden)

include_external_msproject(libeden ${CMAKE_CURRENT_SOURCE_DIR}/libeden/libeden.vcxproj)
include_external_msproject(libeden_test ${CMAKE_CURRENT_SOURCE_DIR}/libeden_test/libeden_test.vcxproj libeden)

# These projects are generated using the Projucer; run the Projucer first if they are missing
include_external_msproject(SharedCode ${CMAKE_CURRENT_SOURCE_DIR}/Builds/VisualStudio2022/EdenSynth_SharedCode.vcxproj libeden)
include_external_msproject(vst3 ${CMAKE_CURRENT_SOURCE_DIR}/Builds/VisualStudio2022/EdenSynth_VST3.vcxproj SharedCode libeden)
include_external_msproject(aax ${CMAKE_CURRENT_SOURCE_DIR}/Builds/VisualStudio2022/EdenSynth_AAX.vcxproj SharedCode libeden)

add_subdirectory(SharedCode_test)
