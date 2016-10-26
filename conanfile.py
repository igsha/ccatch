from conans import ConanFile, CMake

class CCatchConan(ConanFile):
    name = "ccatch"
    version = "0.1.0"
    settings = None
    exports = ["FindCCatch.cmake"]
    url = "https://github.com/igsha/ccatch"
    license = "MIT"

    def build(self):
        cmake = CMake(self.settings)
        self.run('cmake %s %s' % (self.conanfile_directory, cmake.command_line))
        self.run("cmake --build . %s" % cmake.build_config)
        self.run("ctest")

    def package(self):
        self.copy("*.h", dst="include", src="include")
        self.copy("FindCCatch.cmake", ".", ".")
