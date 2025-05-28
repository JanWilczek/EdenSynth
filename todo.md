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
- [ ] Fix too long AAX parameter names
- [ ] Add project CMake aliases::
- [ ] Fix compilation on Mac
- [ ] Make a cross-platform default preset
- [ ] Remove all non-real-time safe operations from the audio thread
- [ ] Make UI resizable
- [ ] Write waveshaping function to presets
- [ ] Add Ninja generator
- [ ] Make UI prettier

## Fix saving presets

- [ ] Fix lack of *assets* folder disallowing OscillatorContainer to add necessary parameters!
  - [ ] Generate a placeholder wavetable
- [ ] Move parameter IDs to a common file
- [ ] Remove `create*OsccillatorSource()` calls from `updateOscillatorParameters()`!
- [ ] Add waveshaping transfer function getter
  - Where?
- [ ] Write a unit test that changes the waveshaping curve and checks if it has been written
- [ ] Add saving and restoring the waveshaping curve in the PluginProcessor
- [ ] Change PresetSaver implementation to use get/set state on a PluginProcessor instance

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
