cmake_minimum_required(VERSION 3.22 FATAL_ERROR)
project(EdenSynth VERSION 1.0.0)

find_package(JUCE CONFIG REQUIRED)

if(MSVC)
  juce_set_aax_sdk_path(${CMAKE_CURRENT_SOURCE_DIR}/../SDKs/AAX)
endif()

if(MSVC)
  set(FORMATS AAX VST3 Standalone)
else()
  set(FORMATS VST3 Standalone)
endif()

if(CMAKE_BUILD_TYPE MATCHES DEBUG)
  message("debug mode")
  set(PLUGIN_VERSION "0.0.0")
else()
  set(PLUGIN_VERSION ${PROJECT_VERSION})
endif()

juce_add_plugin(
  EdenSynth
  PLUGIN_VERSION
  ${PLUGIN_VERSION}
  COMPANY_NAME
  "WolfSound"
  PLUGIN_CODE
  "Esyn"
  PLUGIN_MANUFACTURER_CODE
  "Wfsd"
  BUNDLE_ID
  "com.thewolfsound.edensynth"
  FORMATS
  "${FORMATS}"
  IS_SYNTH
  TRUE
  NEEDS_MIDI_INPUT
  TRUE
  NEEDS_MIDI_OUTPUT
  TRUE
  COPY_PLUGIN_AFTER_BUILD
  TRUE)

target_include_directories(EdenSynth PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_definitions(EdenSynth PUBLIC JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0 JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(
  EdenSynth
  PRIVATE juce::juce_audio_utils
  PUBLIC libeden
         juce::juce_recommended_config_flags
         juce::juce_recommended_lto_flags
         juce::juce_recommended_warning_flags)

juce_generate_juce_header(EdenSynth)

# Re-export include directories to ensure that
# targets linking to EdenSynth will build JUCE modules with the identical options.
target_include_directories(EdenSynth INTERFACE $<TARGET_PROPERTY:EdenSynth,INCLUDE_DIRECTORIES>)

set(SOURCES
    include/Visibility.h
    include/PresetSavingResult.h
    include/PresetLoadingResult.h
    include/ErrorDialogListener.h
    include/viewmodels/PresetsViewModel.h
    include/viewmodels/ViewModel.h
    include/EnvelopeSegmentComponent.h
    include/FilterEnvelopeComponent.h
    include/FilterComponent.h
    include/GeneratorComponent.h
    include/OscillatorComponent.h
    include/GeneralSettingsComponent.h
    include/FilterControlsComponent.h
    include/OutputSettingsComponent.h
    include/PresetsComponent.h
    include/ModifierComponent.h
    include/FilterEnvelopeSegmentComponent.h
    include/WaveshaperComponent.h
    include/OscillatorSourceComponent.h
    include/WaveshapingCanvas.h
    include/OscillatorsComponent.h
    include/EnvelopeComponent.h
    include/EdenAdapter.h
    include/EdenFilterParameters.h
    include/EdenWaveshapingParameters.h
    include/FileHelper.h
    include/OscillatorContainer.h
    include/PluginEditor.h
    include/PluginProcessor.h
    include/PresetManager.h
    include/Presets.h
    include/ProductionPresetManager.h
    include/WaveshapingTransferFunctionContainer.h
    include/WaveTablePathProvider.h
    source/ProductionPresetManager.cpp
    source/viewmodels/PresetsViewModel.cpp
    source/EnvelopeComponent.cpp
    source/EnvelopeSegmentComponent.cpp
    source/FilterComponent.cpp
    source/FilterControlsComponent.cpp
    source/FilterEnvelopeComponent.cpp
    source/OscillatorComponent.cpp
    source/OscillatorsComponent.cpp
    source/OscillatorSourceComponent.cpp
    source/OutputSettingsComponent.cpp
    source/PresetsComponent.cpp
    source/WaveshaperComponent.cpp
    source/WaveshapingCanvas.cpp
    source/FilterEnvelopeSegmentComponent.cpp
    source/GeneralSettingsComponent.cpp
    source/GeneratorComponent.cpp
    source/ModifierComponent.cpp
    source/EdenAdapter.cpp
    source/EdenFilterParameters.cpp
    source/EdenWaveshapingParameters.cpp
    source/FileHelper.cpp
    source/OscillatorContainer.cpp
    source/PluginEditor.cpp
    source/PluginProcessor.cpp
    source/Presets.cpp
    source/WaveshapingTransferFunctionContainer.cpp
    source/WaveTablePathProvider.cpp)

target_sources(EdenSynth PRIVATE ${SOURCES})

source_group(
  TREE ${CMAKE_CURRENT_SOURCE_DIR}
  PREFIX "Source Files"
  FILES ${SOURCES})
