#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from __future__ import absolute_import
from __future__ import division
from __future__ import print_function
from __future__ import unicode_literals

from hilma import Mesh, loadPly, savePly

mesh = Mesh()
loadPly("head.ply", mesh)

verts = []
faces = []
edges = []
uvs = []


for v in mesh.getVertices():
    verts.append( (v.x, v.y, v.z) )

for t in mesh.getTrianglesIndices():
    faces.append( (t.x, t.y, t.z ) )

for e in mesh.getLinesIndices():
    edges.append( (e.x, e.y) )

# print( verts )
# print( faces )
# print(edges)

savePly("out.ply", mesh, False)

