from conans import ConanFile, CMake, tools


class NCConan(ConanFile):
    name = "nc"
    version = "1.0"
    license = "MIT"
    author = "hi-artem"
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}
    generators = "cmake"
    requires = "boost/1.75.0"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC
    
    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder="src")
        cmake.build()

