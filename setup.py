from setuptools import setup
import pybind11
from pybind11.setup_helpers import Pybind11Extension
from glob import glob

# ext_modules = [
#     Extension("ePIE",
#               ["src/ePIE.cpp"],
#               include_dirs = [pybind11.get_include()],
#               language = "c++")]

ext_modules = [
    Pybind11Extension(
        "ePIE",
        sorted(glob("src/*.cpp")),
        include_dirs = [pybind11.get_include()],
        language = "c++",
        cxx_std = 11,
        extra_compile_args = ["-Wall"])]

setup(name = "ePIE", ext_modules=ext_modules)
