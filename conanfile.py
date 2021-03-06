import re

from conans import ConanFile, CMake
from conans.tools import load, check_min_cppstd


def read_project_property(property):
    try:
        cmake_lists = load("CMakeLists.txt")
        value = re.search(r"project\(.*{} \"(.*?)\".*?\)".format(property), cmake_lists, re.S).group(1)
        return value.strip()
    except:
        return None


class CPPFCGIConan(ConanFile):
    name = "cppfcgi"
    scm = {
        "type": "git",
        "url": "https://github.com/fmorgner/cppfcgi.git",
        "revision": "auto",
    }
    version = read_project_property("VERSION")
    license = "BSD-3-Clause"
    url = read_project_property("HOMEPAGE_URL")
    description = read_project_property("DESCRIPTION")
    settings = "arch", "build_type", "compiler", "os"
    options = {"shared": [False, True]}
    default_options = {"shared": False}
    
    def _configure_cmake(self):
        check_min_cppstd(self, 17, gnu_extensions=False)

        cmake = CMake(self)
        cmake.definitions["cppfcgi_RUN_TESTS_AFTER_BUILD"] = False
        cmake.configure()
        return cmake

    def build(self):
        cmake = self._configure_cmake()
        cmake.build()
        cmake.test()

    def package(self):
        cmake = self._configure_cmake()
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["cppfcgi"]
