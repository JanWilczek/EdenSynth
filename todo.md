╔═════════════════════════════════════════╗
║    ✅ / 🧠  LOCAL TODO FILE 🧠 / ✅     ║
╚═════════════════════════════════════════╝

'/Library/Arturia/Presets/Pigments/Factory/Pigments 6.0/Analog Reverie'
/Library/Arturia/Presets/Pigments/User/jawi/Foo1
/Library/Arturia/Pigments/resources/internal_presets
/Library/Application Support/Native Instruments/Massive X/Presets
User presets folder:
'/Users/jawi/Documents/Native Instruments/User Content/Massive X/Presets'

Plugin folder: /Library/WolfSound/EdenSynth
Inside:
    - wavetables/
    - presets/

User presets folder:
/Users/jawi/Documents/WolfSound/EdenSynth/presets

## Fix saving presets

- [x] Copy assets folder to '/Library/Application Support/WolfSound/EdenSynth'
- [x] **Change PresetSaver implementation to use get/set state on a PluginProcessor instance**
    - [x] Add an interface to PluginProcessor -> StateHolder
    - [x] The implementer should redirect to get/setStateInformation
    - [x] Pass StateHolder to PresetManager
    - [x] Refactor FileHelper.cpp
- [x] Combine system and user paths in the Presets class
- [ ] Fix parameters loaded as presets being treated as strings (?) -> the APVTS parameters are incorrectly updated.
    - [ ] Add an AudioParameterChoice to 2 parameter tests
    - [ ] Add an AudioParameterInt to 2 parameter tests
- [ ] Fix lack of *assets* folder disallowing OscillatorContainer to add necessary parameters!
  - [ ] Generate a placeholder wavetable
- [x] Save user presets in a user folder
- [ ] Remove `create*OsccillatorSource()` calls from `updateOscillatorParameters()`!
- [ ] (Write a unit test that changes the waveshaping curve and checks if it has been written)
- [x] Find a way to display the assets path error in the editor not in the processor
- [ ] Bundle factory presets with the plugin as binary data and return them as "programs"
- [ ] Use seed value from the parameter when generating the waveshaping curve

```cpp
    AlertWindow::showMessageBoxAsync(
        AlertWindow::AlertIconType::WarningIcon, "Assets error",
        "Assets folder not found. The assets folder should be in the same "
        "directory as the plugin. The synthesiser won't play.",
        "OK");

```

```xml
<Waveshaper Spread="0.5" SpreadSeed="1" SelectedCurve="HyperbolicTangent" ChebyshevPolynomialOrder="2" />
```

## Misc

- [ ] Write unit test for pathToExistingPreset (directory_iterator may throw)
- [ ] Add GitHub actions
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


## New UI Design

- [ ] Create a new (flat) design in Figma
    - [ ] Light theme
    - [ ] Dark theme
    - [ ] Decide on the font
- [ ] Implement the design using an LnF implementer + color theme
    - [ ] Don't use images


```cpp

void getStateInformation() {
	juce::MemoryOutputStream outStream{...};
	serializer->serialize(outStream, parameters);
}

void setStateInformation() {
	juce::MemoryInputStream inStream{...};
	serializer->deserialize(inStream, parameters /* ref */);
}

//====

void getStateInformation() {
	const ?? rawParameterRepresentation = serializer->serialize(parameters);
	// write rawParameterRepresentation
}

void setStateInformation() {
	juce::MemoryInputStream inStream{...};
	const auto parameterRepresentation = serializer->deserialize(inStream);
	// map parameterRepresentation to parameters
}

```
