╔═════════════════════════════════════════╗
║    ✅ / 🧠  LOCAL TODO FILE 🧠 / ✅     ║
╚═════════════════════════════════════════╝

- [x] Transition libeden_test to CMake
  - [x] Add source files grouping
- [x] Write more preset tests
- [x] Bump googletest version
- [x] Check on Mac
- [ ] Write unit test for pathToExistingPreset (directory_iterator may throw)
- [ ] Add GitHub actions
- [x] Bump JUCE version
- [x] Fix too long AAX parameter names
- [ ] Add project CMake aliases::
- [ ] Fix compilation on Mac
- [ ] Make a cross-platform default preset
- [ ] Remove all non-real-time safe operations from the audio thread
- [ ] Make UI resizable
- [ ] Write waveshaping function to presets
- [ ] Add Ninja generator
- [ ] Make UI prettier
- [ ] Move parameter IDs to a common file | provide a tree-based parameter class abstraction
- [ ] Fix the waveshaping transfer function data race in WaveshapingTransferFunctionContainer

## Fix saving presets

- [ ] Fix lack of *assets* folder disallowing OscillatorContainer to add necessary parameters!
  - [ ] Generate a placeholder wavetable
- [ ] Remove `create*OsccillatorSource()` calls from `updateOscillatorParameters()`!
- [x] Add saving and restoring the waveshaping curve in the PluginProcessor -> that does not seem to be a good way to save the curve. We should
    - [ ] Save exact control values including the random seed to a ValueTree:
        - spread value
        - spread seed
        - selected curve (AvailableCurves enum)
        - Chebyshev polynomial order
    - [ ] Find a way to attach the properties of the ValueTree to GUI controls easily
        - [ ] Create a struct called `WaveshapingParameters`
        - [ ] Define `SerialisationTraits` for it so that it's properly convertible to `juce::var`
            - [ ] This can be easily unit-tested
        - [ ] PluginProcessor should create this ValueTree node and attach it to APVTS in its constructor
        - [ ] Write a unit test that changes the waveshaping curve and checks if it has been written
        - [ ] It should be read and written in set/getStateInformation()
        - [ ] It should be passed to the WaveshaperComponent in the constructor
        - [ ] WaveshaperComponent should observe it and react accordingly
        - [ ] WaveshaperComponent should update this ValueTree's properties according to GUI controls
- [ ] Change PresetSaver implementation to use get/set state on a PluginProcessor instance

```xml
<Waveshaper Spread="0.5" SpreadSeed="1" SelectedCurve="HyperbolicTangent" ChebyshevPolynomialOrder="2" />
```

## Migration to CMake

- [x] Should SharedCode/include be PUBLIC for EdenSynth?
- [x] Should EdenSynth link to JUCE targets related to flag and builds?
- [x] Should EdenSynth link to JUCE stuff privately? -> I understand so
- [ ] Warning flags could be more strict on all targets
- [ ] Is juce_set_aax_sdk_path() needed if I use JUCE 8.0.7?

- [ ] Fix the following error when building:

```bash
JUCE v8.0.7
*** Leaked objects detected: 1 instance(s) of class AlertWindowImpl
JUCE Assertion failure in juce_LeakedObjectDetector.h:104
*** Leaked objects detected: 1 instance(s) of class AsyncUpdater
JUCE Assertion failure in juce_LeakedObjectDetector.h:104
*** Leaked objects detected: 1 instance(s) of class StringArray
JUCE Assertion failure in juce_LeakedObjectDetector.h:104
```
