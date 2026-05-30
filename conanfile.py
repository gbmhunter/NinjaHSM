import os

from conan import ConanFile
from conan.tools.build import check_min_cppstd
from conan.tools.files import copy


class NinjaHSMConan(ConanFile):
    name = "ninjahsm"
    version = "1.4.0"
    description = (
        "A small, simple hierarchical state machine (HSM) library for C++ embedded systems. "
        "Header-only, no dynamic memory allocation."
    )
    license = "MIT"
    author = "Geoff Hunter <gbmhunter@gmail.com>"
    url = "https://github.com/gbmhunter/NinjaHSM"
    homepage = "https://github.com/gbmhunter/NinjaHSM"
    topics = ("state-machine", "hsm", "statechart", "embedded", "fsm", "hierarchical", "header-only")

    package_type = "header-library"
    settings = "os", "arch", "compiler", "build_type"
    no_copy_source = True

    exports_sources = "src/*", "LICENSE"

    def requirements(self):
        # NinjaHSM's public headers include <etl/delegate.h>, so ETL must be visible to consumers.
        self.requires("etl/20.39.4", transitive_headers=True)

    def validate(self):
        check_min_cppstd(self, 17)

    def package_id(self):
        self.info.clear()

    def package(self):
        copy(
            self,
            "*.hpp",
            src=os.path.join(self.source_folder, "src"),
            dst=os.path.join(self.package_folder, "include"),
        )
        copy(
            self,
            "LICENSE",
            src=self.source_folder,
            dst=os.path.join(self.package_folder, "licenses"),
        )

    def package_info(self):
        # Header-only: nothing to link.
        self.cpp_info.bindirs = []
        self.cpp_info.libdirs = []
