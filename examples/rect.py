#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from __future__ import absolute_import
from __future__ import division
from __future__ import print_function
from __future__ import unicode_literals

from hilma import Mesh, PlyOps

import numpy as np
from math import ceil

def tessRect( mesh, width, height, precision = 1.0, z = 0.0, color = None):
    offset = mesh.getVerticesTotal()
    normal = [0., 0., 1.]

    w = ceil(width / precision)
    h = ceil(height / precision)
    for y in range(0, int(h)):
        for x in range(0, int(w)):
            mesh.addVertex( [float(x) * precision - float(width) * 0.5, 
                             float(y) * precision - float(height) * 0.5, 
                             z] )
            if color:
                mesh.addColor( color )
            mesh.addNormal( normal )
            mesh.addTexCoord( [float(x)/float(w-1), float(y)/float(h-1)] )
    
    for y in range(0, int(h)-1):
        for x in range(0, int(w)-1):
            if x%2 == 0:
                mesh.addTriangle(   offset + x + y * w,               # a
                                    offset + (x + 1) + y * w,         # b
                                    offset + x + (y + 1) * w)         # d
                
                mesh.addTriangle(   offset + (x + 1) + y * w,         # b
                                    offset + (x + 1) + (y + 1) * w,   # c
                                    offset + x + (y + 1) * w)         # d
            else:
                mesh.addTriangle(   offset + (x + 1) + (y + 1) * w,   # c
                                    offset + x + y * w,               # a
                                    offset + (x + 1 ) + y * w)        # b
                
                mesh.addTriangle(   offset + (x + 1) + (y + 1) * w,   # c
                                    offset + x + (y + 1) * w,         # d
                                    offset + x + y * w)               # a
    return mesh


mesh = Mesh()
tessRect(mesh, 10, 10)
PlyOps.save("rect.ply", mesh, False)

