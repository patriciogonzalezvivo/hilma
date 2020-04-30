#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from __future__ import absolute_import
from __future__ import division
from __future__ import print_function
from __future__ import unicode_literals

from hilma import Mesh, PlyOps
import numpy

def rect2D(x, y, width, height, centered = False):
    left    = x
    right   = x + width
    bottom  = y
    top     = y + height

    if centered:
        left    = x-width/2
        right   = x+width/2
        bottom  = y-height/2
        top     = y+height/2
    
    return [ [left,bottom], [left,top], [right,top], [right,bottom] ]

size = 1024
mesh = Mesh()
mesh.addVertices( rect2D(0, 0, size, size, True) )
PlyOps.save("quad.ply", mesh, False)