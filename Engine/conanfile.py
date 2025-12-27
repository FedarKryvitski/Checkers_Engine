from conan import ConanFile
from conan.tools.cmake import cmake_layout, CMake

class ExampleRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires("sfml/3.0.2")
        self.requires("nlohmann_json/3.11.3")
        self.requires("spdlog/1.12.0")

    def layout(self):
        cmake_layout(self)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()