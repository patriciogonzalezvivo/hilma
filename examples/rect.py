#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from __future__ import absolute_import
from __future__ import division
from __future__ import print_function
from __future__ import unicode_literals

from hilma import Mesh, savePly
import numpy as np


size = 1024
half = size/2.0


# 1 -- 2  
# |    |
# 0 -- 3

mesh_A = Mesh()
mesh_A.addVertices( [ [-half, -half], [-half, half], [half, half], [half, -half] ] )
mesh_A.addIndices( [ 1, 0, 3 , 1, 3, 2] )
savePly("rect_A.ply", mesh_A, False)

mesh_B = Mesh()
mesh_B.addVertices( [ (-half, -half, 0.0), (-half, half, 0.0), (half, half, 0.0), (half, -half, 0.0) ] )
mesh_B.addFaces( [ (1, 0, 3 ), (1, 3, 2)] ) 
savePly("rect_B.ply", mesh_B, False)

mesh_C = Mesh()
mesh_C.addVertices( [ [-half, -half, 0.0], [-half, half, 0.0], [half, half, 0.0], [half, -half, 0.0] ] )
mesh_C.addFaces( [ [1, 0, 3] , [1, 3, 2] ] ) 
savePly("rect_C.ply", mesh_C, False)

mesh_D = Mesh()
mesh_D.addVertices( np.array([ [-half, -half, 0.0], [-half, half, 0.0], [half, half, 0.0], [half, -half, 0.0] ], dtype=np.float32) )
mesh_D.addFaces( np.array([ [1, 0, 3], [1, 3, 2] ], dtype=np.int32) ) 
savePly("rect_D.ply", mesh_D, False)

mesh_E = Mesh()
mesh_E.addVertex( [-half, -half, 0.0] )
mesh_E.addVertex( [-half, half, 0.0] ) 
mesh_E.addVertex( [half, half, 0.0] )
mesh_E.addVertex( [half, -half, 0.0] )
mesh_E.addTriangle( 1, 0, 3 )
mesh_E.addTriangle( 1, 3, 2 )
savePly("rect_E.ply", mesh_E, False)


