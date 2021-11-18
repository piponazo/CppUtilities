from conans import ConanFile
from conans.tools import os_info
from conans.model.version import Version

class CppUtilities(ConanFile):
    settings = 'os', 'compiler', 'build_type', 'arch'
    generators = 'cmake_find_package'

    def requirements(self):
        self.requires('catch2/2.13.7')

    def imports(self):
        self.copy('*.dll', dst='conanDlls', src='bin')
        self.copy('*.dylib', dst='bin', src='lib')

