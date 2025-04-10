from setuptools import setup, Extension
import pybind11

ext_modules = [
    Extension("ePIE",
              ["src/ePIE.cpp"],
              include_dirs = [pybind11.get_include()],
              language = "c++")]

setup(name = "ePIE", ext_modules=ext_modules)
