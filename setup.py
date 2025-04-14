import os
import glob
import pybind11
import numpy
from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension

vcpkg_root = os.path.expanduser("~/vcpkg/installed/x64-linux")

include_dirs = [
    pybind11.get_include(),
    numpy.get_include(),
    os.path.join(vcpkg_root, "include"),
]

library_dirs = [
    os.path.join(vcpkg_root, "lib")
]

ext_modules = [
    Pybind11Extension(
        name = "libFrePIE",
        sources = sorted(glob.glob("src/*.cpp")), # sorted for reproducibility
        include_dirs = include_dirs,
        library_dirs = library_dirs,
        libraries = ["fftw3"],
        language = "c++",
        cxx_std = 17,
        extra_compile_args = ["-Wall"])]

setup(name = "libFrePIE", ext_modules = ext_modules)
