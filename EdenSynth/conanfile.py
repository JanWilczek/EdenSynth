# Run with
# conan install . -s build_type=Debug --build=missing -s compiler.runtime_type=Debug
# conan install . --build=missing -s build_type=Release -s compiler.runtime_type=Release
from conan import ConanFile
from conan.tools.microsoft import MSBuildDeps
from conan.tools.cmake import CMakeDeps, CMakeToolchain, cmake_layout


class EdenSynthRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"

    def requirements(self):
        self.requires("juce/7.0.3")
        self.test_requires("gtest/cci.20210126")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        ms = MSBuildDeps(self)
        ms.generate()

        cmake = CMakeDeps(self)
        cmake.generate()

        tc = CMakeToolchain(self)
        tc.user_presets_path = "ConanPresets.json"
        tc.generate()
