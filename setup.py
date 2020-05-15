#!/usr/bin/env python

"""
hilma: is a library that:
- simple C++ interfaces
- minimal dependences so the C++ code can be added to other C++ projects easily
- add Python package which makes a C++ wrapper usign swig
"""

from distutils.core import setup, Extension

# Third-party modules - we depend on numpy for everything
import numpy

# Obtain the numpy include directory.  This logic works across numpy versions.
try:
    numpy_include = numpy.get_include()
except AttributeError:
    numpy_include = numpy.get_numpy_include()

doc_lines = __doc__.split('\n')
hilma_module = Extension(  
  '_hilma',
  include_dirs=['include', numpy_include],
  sources= [ 
    'hilma_wrap.cxx',
    'src/types/Polyline.cpp',
    'src/types/Triangle.cpp',
    'src/types/Mesh.cpp',
    'src/ops/compute.cpp',
    'src/ops/generate.cpp',
    'src/ops/convert.cpp',
    'src/ops/transform.cpp',
    'src/ops/intersection.cpp',
    'src/io/obj.cpp',
    'src/io/ply.cpp',
    'src/io/stl.cpp',
   ],
   swig_opts = ['-c++']
)

setup(  
  name = 'hilma',
  description = doc_lines[0],
  long_description = '\n'.join(doc_lines[2:]),
  version     = '0.1',
  author      = 'Patricio Gonzalez Vivo',
  author_email = 'patriciogonzalezvivo@gmail.com',
  license     = "BSD",
  ext_modules = [ hilma_module ],
  py_modules = [ 'hilma' ],
)
