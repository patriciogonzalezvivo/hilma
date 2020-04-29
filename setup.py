#!/usr/bin/env python

"""
hilma: is a library that:
- simple C++ interfaces
- minimal dependences so the C++ code can be added to other C++ projects easily
- add Python package which makes a C++ wrapper usign swig
"""

from distutils.core import setup, Extension

doc_lines = __doc__.split('\n')
hilma_module = Extension(  
  '_hilma',
  include_dirs=['include'],
  sources= [ 
    'hilma_wrap.cxx',
    'src/Mesh.cpp',
    'src/Material.cpp',
    'src/io/ObjOps.cpp',
    'src/io/PlyOps.cpp',
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
