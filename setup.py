   #----------------------------------------------------------------------#
   #  distutils setup script for compiling cut-pursuit python extensions  #
   #----------------------------------------------------------------------#
""" 
Compilation command: python setup.py build_ext

Hugo Raguet 2020
"""

from setuptools import setup, Extension
import numpy
import platform

###  targets and compile options  ###
name = "grid_graph"

include_dirs = [numpy.get_include(), # find the Numpy headers
                "include"]
# compilation and linkage options
# MIN_OPS_PER_THREAD roughly controls parallelization, see doc in README.md
if platform.system() == "Windows":
    extra_compile_args = ["-DMIN_OPS_PER_THREAD=10000"]
    extra_link_args = ["/lgomp"]
elif platform.system() == "Linux":
    extra_compile_args = ["-std=c++11", "-fopenmp",
                          "-DMIN_OPS_PER_THREAD=10000"]
    extra_link_args = ["-lgomp"]
elif platform.system() == "Darwin":
    extra_compile_args = ["-std=c++11", "-fopenmp",
                          "-DMIN_OPS_PER_THREAD=10000"]
    extra_link_args = ["-lomp"] 
# It is more a matter of GCC vs. Clang more than a macOS vs Linux issue
# use something like meson/cmake to handle this in a proper way
else:
    raise NotImplementedError("OS not yet supported.")

###  compilation  ###
mod = Extension(
        name,
        # list source files
        ["python/cpython/grid_graph_cpy.cpp",
         "src/edge_list_to_forward_star.cpp",
         "src/grid_to_graph.cpp"],
        include_dirs=include_dirs,
        extra_compile_args=extra_compile_args,
        extra_link_args=extra_link_args)

setup(name=name, ext_modules=[mod])
