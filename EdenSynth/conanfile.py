# Run with
# conan install . -s build_type=Debug --build=missing -s compiler.runtime_type=Debug
# conan install . --build=missing -s build_type=Release -s compiler.runtime_type=Release
from conan import ConanFile
from conan.tools.microsoft import MSBuildDeps

class EdenSynthRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    
    def requirements(self):
        self.test_requires("gtest/cci.20210126")
        
    def layout(self):
        output_folder = "dependencies"
        self.folders.build = output_folder
        self.folders.generators = output_folder
        
    def generate(self):
        ms = MSBuildDeps(self)
        ms.generate()
