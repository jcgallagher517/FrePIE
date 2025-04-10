import glob
import pybind11
from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension

ext_modules = [
    Pybind11Extension(
        name = "FrePIE",
        sources = sorted(glob.glob("src/*.cpp")), # sorted for reproducibility
        include_dirs = [pybind11.get_include()],
        language = "c++",
        cxx_std = 17,
        extra_compile_args = ["-Wall"])]

setup(name = "FrePIE", ext_modules = ext_modules)
