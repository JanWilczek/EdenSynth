# EdenSynth by Jan Wilczek

## Features

* wavetable, Virtual Analog, subtractive, and waveshaping modules,
* physically meaningful controls,
* fine-tuned control over parameters.

## Build instructions (Visual Studio 2022)

To build this project

1. Generate the Visual Studio 2022 project by invoking the proper generator with the Projucer (*EdenSynth.jucer* file). Use version 7.0.3 of [JUCE](https://juce.com/).
1. Open *eden.sln* solution in Visual Studio 2022 and build the solution.
1. *(optionally for the AAX build)* Copy the AAX SDK into *EdenSynth/SDKs/AAX* folder.
1. *(optionally for developers)* Run `conan create .` in *dependency_wrappers/juce* directory. This will create the JUCE Conan package on your system. You need to have Conan 2.0 installed.
1. *(optionally for developers)* Run `conan install . -s build_type=Debug --build=missing -s compiler.runtime_type=Debug` and `conan install . --build=missing -s build_type=Release -s compiler.runtime_type=Release` in the EdenSynth  directory. You need to have Conan 2.0 installed.
1. *(optionally for developers)* Run `pre-commit install` in the main directory. You need to have [pre-commit](https://pre-commit.com/) installed.

## 📝 License

The code in this repository is licensed under the GNU Lesser General Public License (LGPL) v3. Commercial Non-LGPL license alternative available upon request.

JUCE C++ framework is separately licensed.

