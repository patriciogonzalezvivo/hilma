#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from __future__ import absolute_import
from __future__ import division
from __future__ import print_function
from __future__ import unicode_literals

from hilma import Mesh, savePly

import numpy as np
from math import cos, sin, sqrt


def normalize(v, tolerance=0.00001):
    mag2 = sum(n * n for n in v)
    if abs(mag2 - 1.0) > tolerance:
        mag = sqrt(mag2)
        v = tuple(n / mag for n in v)
    return np.array(v)

# Port from C++ https://bitbucket.org/transporter/ogre-procedural/src/ca6eb3363a53c2b53c055db5ce68c1d35daab0d5/library/src/ProceduralIcoSphereGenerator.cpp?at=default&fileviewer=file-view-default
def icosphere(mesh, radius=1, resolution=2, color=None):

    # Step 1 : Generate icosahedron
    sqrt5 = sqrt(5.0);
    phi = (1.0 + sqrt5) * 0.5;
    invnorm = 1.0/sqrt(phi*phi+1.0);

    mesh.addVertex(invnorm * np.array([-1,  phi, 0]))  #0
    mesh.addVertex(invnorm * np.array([ 1,  phi, 0]))  #1
    mesh.addVertex(invnorm * np.array([0,   1,  -phi]))#2
    mesh.addVertex(invnorm * np.array([0,   1,   phi]))#3
    mesh.addVertex(invnorm * np.array([-phi,0,  -1]))  #4
    mesh.addVertex(invnorm * np.array([-phi,0,   1]))  #5
    mesh.addVertex(invnorm * np.array([ phi,0,  -1]))  #6
    mesh.addVertex(invnorm * np.array([ phi,0,   1]))  #7
    mesh.addVertex(invnorm * np.array([0,   -1, -phi]))#8
    mesh.addVertex(invnorm * np.array([0,   -1,  phi]))#9
    mesh.addVertex(invnorm * np.array([-1,-phi, 0]))  #10
    mesh.addVertex(invnorm * np.array([ 1,-phi, 0]))  #11

    if color:
        for i in range(12):
            mesh.addColor( color )
         
    firstFaces = [
        0,1,2,
        0,3,1,
        0,4,5,
        1,7,6,
        1,6,2,
        1,3,7,
        0,2,4,
        0,5,3,
        2,6,8,
        2,8,4,
        3,5,9,
        3,9,7,
        11,6,7,
        10,5,4,
        10,4,8,
        10,9,5,
        11,8,6,
        11,7,9,
        10,8,11,
        10,11,9
    ]

    for i in range(0, 60)[::3]:
        mesh.addTriangle(firstFaces[i], firstFaces[i+1], firstFaces[i+2])

    size = mesh.getIndicesTotal() # len(mesh.indices)
    # Step 2: tessellate
    for iteration in range(0, resolution):
        size*=4;
        newFaces = []
        for i in range(0, int(size/12)):
            i1 = mesh.indices[i*3]
            i2 = mesh.indices[i*3+1]
            i3 = mesh.indices[i*3+2]

            i12 = len(mesh.vertices)
            i23 = i12 + 1
            i13 = i12 + 2

            v1 = mesh.vertices[i1]
            v2 = mesh.vertices[i2]
            v3 = mesh.vertices[i3]

            # make 1 vertice at the center of each edge and project it onto the mesh
            mesh.vertices.append(np.array(normalize( v1 + v2 )))
            mesh.vertices.append(np.array(normalize( v2 + v3 )))
            mesh.vertices.append(np.array(normalize( v1 + v3 )))

            if color:
                for i in range(3):
                    mesh.addColor( color )

            # now recreate indices
            newFaces.append(i1)
            newFaces.append(i12)
            newFaces.append(i13)
            newFaces.append(i2)
            newFaces.append(i23)
            newFaces.append(i12)
            newFaces.append(i3)
            newFaces.append(i13)
            newFaces.append(i23)
            newFaces.append(i12)
            newFaces.append(i23)
            newFaces.append(i13)

        mesh.indices = list(newFaces);
    
    #  Step 3 : generate texcoords
    texCoords = []
    for i in range(0, len(mesh.vertices)):
        vec = mesh.vertices[i]
        r0 = sqrt(vec[0] * vec[0] + vec[2] * vec[2])
        alpha = atan2(vec[2], vec[0])

        u = alpha/TAU + .5;
        v = atan2(vec[1], r0)/np.pi + .5;

        # reverse the u coord, so the default is texture mapped left to
        # right on the outside of a sphere 
        # reverse the v coord, so that texture origin is at top left
        texCoords.append( np.array( [1.0-u, v ] ))

    # Step 4 : fix texcoords
    # find vertices to split
    indexToSplit = []
    for i in range(0, int(len(mesh.indices)/3)):
        t0 = texCoords[ mesh.indices[i*3+0] ]
        t1 = texCoords[ mesh.indices[i*3+1] ]
        t2 = texCoords[ mesh.indices[i*3+2] ]

        if abs(t2[0]-t0[0]) > 0.5:
            if t0[0] < 0.5:
                indexToSplit.append( mesh.indices[i*3] )
            else:
                indexToSplit.append( mesh.indices[i*3+2] )
        
        if abs(t1[0]-t0[0]) > 0.5:
            if t0[0] < 0.5:
                indexToSplit.append( mesh.indices[i*3] )
            else:
                indexToSplit.append( mesh.indices[i*3+1] )
        
        if abs(t2[0]-t1[0]) > 0.5:
            if t1[0] < 0.5:
                indexToSplit.append( mesh.indices[i*3+1] )
            else:
                indexToSplit.append( mesh.indices[i*3+2] )

    # split vertices
    for i in range(0, int(len(indexToSplit)/3)):
        index = indexToSplit[i]
        # duplicate vertex
        v = mesh.vertices[index]
        t = texCoords[index] + np.array( [1., 0.] )

        mesh.vertices.append(v)
        texCoords.append(t)

        if color:
           mesh.addColor( color )
        
        newIndex = len(mesh.vertices)-1

        # reassign indices
        for j in range(len(mesh.indices)):
            if mesh.indices[j] == index:
                index1 = mesh.indices[ int((j+1)%3+(j/3)*3) ]
                index2 = mesh.indices[ int((j+2)%3+(j/3)*3) ]
                if (texCoords[index1][0] > 0.5) or (texCoords[index2][0] > 0.5):
                    mesh.indices[j] = newIndex;

    for vert in mesh.vertices:
        mesh.addNormal( normalize(vert) )

    for st in texCoords:
        mesh.addTexCoord( st )

    for i in range(len( mesh.vertices )):
        mesh.vertices[i] = mesh.vertices[i] * radius

    return mesh


mesh = Mesh()
icosphere(mesh, 1, 1)
savePly("icosphere.ply", mesh, False)

