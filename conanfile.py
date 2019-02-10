from conans import ConanFile
from conans.tools import os_info
from conans.model.version import Version

class CppUtilities(ConanFile):
    settings = 'os', 'compiler', 'build_type', 'arch'
    generators = 'cmake'

    def requirements(self):
        self.requires('Catch2/2.6.0@catchorg/stable')

    def imports(self):
        self.copy('*.dll', dst='conanDlls', src='bin')
        self.copy('*.dylib', dst='bin', src='lib')

