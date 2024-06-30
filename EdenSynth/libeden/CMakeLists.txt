cmake_minimum_required(VERSION 3.22 FATAL_ERROR)
project(libeden)

set(SOURCES
    # header files
    include/eden/AudioBuffer.h
    include/eden/EdenSynthesiser.h
    include/eden/EnvelopeParameters.h
    include/eden/FilterParameters.h
    include/eden/MidiBuffer.h
    include/eden/MidiMessage.h
    include/eden/Oscillator.h
    include/eden/OscillatorParameters.h
    include/eden/OscillatorSource.h
    include/eden/WaveshapingFunctionGenerator.h
    include/eden_impl/EdenSynthesiserImpl.h
    include/eden_impl/OscillatorImpl.h
    include/eden_impl/OscillatorSourceImpl.h
    include/eden_impl/WaveshapingFunctionGeneratorImpl.h
    include/interpolation/IInterpolator.h
    include/interpolation/LinearInterpolator.h
    include/settings/EnvelopeSettings.h
    include/settings/GeneratorSettings.h
    include/settings/Settings.h
    include/settings/SubtractiveModuleSettings.h
    include/settings/Tuner.h
    include/settings/WaveshapingModuleSettings.h
    include/synth/envelope/ADBDR.h
    include/synth/envelope/ADSR.h
    include/synth/envelope/Decay.h
    include/synth/envelope/Envelope.h
    include/synth/envelope/EnvelopeFactory.h
    include/synth/envelope/EnvelopeGenerator.h
    include/synth/envelope/EnvelopeSegment.h
    include/synth/envelope/Attack.h
    include/synth/envelope/ExponentialGain.h
    include/synth/envelope/IEnvelopeHolder.h
    include/synth/envelope/ISegmentGain.h
    include/synth/envelope/LinearGain.h
    include/synth/envelope/NoGain.h
    include/synth/envelope/SegmentGainFactory.h
    include/synth/envelope/Sustain.h
    include/synth/IMonoModule.h
    include/synth/Mixer.h
    include/synth/subtractive/MoogFilter.h
    include/synth/subtractive/OnePoleLowPassFilter.h
    include/synth/subtractive/SubtractiveModule.h
    include/synth/Synthesiser.h
    include/synth/Voice.h
    include/synth/waveshaping/Waveshaper.h
    include/synth/waveshaping/WaveshapingModule.h
    include/synth/wavetable/SignalGenerator.h
    include/synth/wavetable/SineWaveTable.h
    include/synth/wavetable/SynthOscillator.h
    include/synth/wavetable/IOscillatorSource.h
    include/synth/wavetable/va_sources/SawtoothVASource.h
    include/synth/wavetable/va_sources/PulseVASource.h
    include/synth/wavetable/va_sources/SineVASource.h
    include/synth/wavetable/va_sources/TriangleVASource.h
    include/synth/wavetable/Waveform.h
    include/synth/wavetable/WaveTable.h
    include/synth/wavetable/WaveTableSource.h
    include/synth/wavetable/WhiteNoiseSource.h
    include/utility/dsp/Correlation.h
    include/utility/EdenAssert.h
    include/utility/MathConstants.h
    include/utility/StopWatch.h
    include/utility/StopWatchPrinter.h
    include/utility/threading/JoinThreads.h
    include/utility/threading/ThreadPool.h
    include/utility/threading/ThreadSafeQueue.h
    include/utility/TimeSampleConverter.h
    include/utility/WaveFileReader.h
    # source files
    source/eden/AudioBuffer.cpp
    source/eden/EdenSynthesiser.cpp
    source/eden/MidiBuffer.cpp
    source/eden/MidiMessage.cpp
    source/eden/Oscillator.cpp
    source/eden/OscillatorSource.cpp
    source/eden/WaveshapingFunctionGenerator.cpp
    source/eden_impl/EdenSynthesiserImpl.cpp
    source/eden_impl/OscillatorImpl.cpp
    source/eden_impl/OscillatorSourceImpl.cpp
    source/eden_impl/WaveshapingFunctionGeneratorImpl.cpp
    source/interpolation/LinearInterpolator.cpp
    source/settings/EnvelopeSettings.cpp
    source/settings/GeneratorSettings.cpp
    source/settings/Settings.cpp
    source/settings/SubtractiveModuleSettings.cpp
    source/settings/Tuner.cpp
    source/settings/WaveshapingModuleSettings.cpp
    source/synth/envelope/ADBDR.cpp
    source/synth/envelope/ADSR.cpp
    source/synth/envelope/Attack.cpp
    source/synth/envelope/Decay.cpp
    source/synth/envelope/Envelope.cpp
    source/synth/envelope/EnvelopeFactory.cpp
    source/synth/envelope/EnvelopeGenerator.cpp
    source/synth/envelope/EnvelopeSegment.cpp
    source/synth/envelope/ExponentialGain.cpp
    source/synth/envelope/LinearGain.cpp
    source/synth/envelope/NoGain.cpp
    source/synth/envelope/SegmentGainFactory.cpp
    source/synth/envelope/Sustain.cpp
    source/synth/Mixer.cpp
    source/synth/subtractive/MoogFilter.cpp
    source/synth/subtractive/OnePoleLowPassFilter.cpp
    source/synth/subtractive/SubtractiveModule.cpp
    source/synth/Synthesiser.cpp
    source/synth/Voice.cpp
    source/synth/waveshaping/Waveshaper.cpp
    source/synth/waveshaping/WaveshapingModule.cpp
    source/synth/wavetable/SignalGenerator.cpp
    source/synth/wavetable/SynthOscillator.cpp
    source/synth/wavetable/va_sources/SawtoothVASource.cpp
    source/synth/wavetable/va_sources/PulseVASource.cpp
    source/synth/wavetable/va_sources/SineVASource.cpp
    source/synth/wavetable/va_sources/TriangleVASource.cpp
    source/synth/wavetable/Waveform.cpp
    source/synth/wavetable/WaveTableSource.cpp
    source/synth/wavetable/WhiteNoiseSource.cpp
    source/utility/StopWatch.cpp
    source/utility/StopWatchPrinter.cpp
    source/utility/threading/JoinThreads.cpp
    source/utility/threading/ThreadPool.cpp
    source/utility/threading/ThreadSafeQueue.cpp
    source/utility/TimeSampleConverter.cpp
    source/utility/WaveFileReader.cpp)

add_library(libeden STATIC ${SOURCES})

target_include_directories(libeden PUBLIC include)

if(MSVC)
  target_compile_options(libeden PRIVATE /W3 /WX /sdl /std:c++latest)
endif()

source_group(
  TREE ${CMAKE_CURRENT_SOURCE_DIR}
  PREFIX "Source Files"
  FILES ${SOURCES})
