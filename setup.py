#!/usr/bin/env python

"""
Hilma is 3D geometry library in C++ with Python bindings (with support for Numpy) ready to be embed into your projects. 
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
    'src/types/Camera.cpp',
    'src/types/Image.cpp',
    'src/types/Material.cpp',
    'src/types/Line.cpp',
    'src/types/Triangle.cpp',
    'src/types/Polygon.cpp',
    'src/types/Polyline.cpp',
    'src/types/Mesh.cpp',
    'src/ops/compute.cpp',
    'src/ops/convert_image.cpp',
    'src/ops/convert_path.cpp',
    'src/ops/generate.cpp',
    'src/ops/transform.cpp',
    'src/ops/intersection.cpp',
    'src/ops/raytrace.cpp',
    'src/io/obj.cpp',
    'src/io/ply.cpp',
    'src/io/stl.cpp',
    'src/io/gltf.cpp',
    'src/io/auto.cpp',
    'src/accel/BVH.cpp',
    'src/accel/KdTree.cpp',
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
