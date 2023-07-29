import os

from conan import ConanFile
from conan.tools.cmake import cmake_layout, CMake, CMakeToolchain


class ImGuiExample(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps"
    package_type = "header-library"
    default_options = {"boost/*:header_only": True}

    def requirements(self):
        self.requires("boost/[^1.62]", headers=True, libs=False, transitive_headers=True)
        
    def build_requirements(self):
        self.test_requires("doctest/[^2.4]")

    def layout(self):
        cmake_layout(self)
        
    def generate(self):
        CMakeToolchain(self).generate()
        
    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        cmake.test()
        
        