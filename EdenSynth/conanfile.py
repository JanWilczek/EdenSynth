# Run with
# conan install . -s build_type=Debug --build=missing -s compiler.runtime_type=Debug -s compiler.cppstd=gnu23
# conan install . --build=missing -s build_type=Release -s compiler.runtime_type=Release -s compiler.cppstd=gnu23
from conan import ConanFile
from conan.tools.microsoft import MSBuildDeps
from conan.tools.cmake import CMakeDeps, CMakeToolchain, cmake_layout


class EdenSynthRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"

    def requirements(self):
        self.requires("juce/8.0.12")
        self.requires("wolfsound-dsp-utils/0.6.1")
        self.test_requires("gtest/1.14.0")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        cmake = CMakeDeps(self)
        cmake.generate()

        tc = CMakeToolchain(self)
        tc.user_presets_path = "ConanPresets.json"
        tc.generate()
