#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from __future__ import absolute_import
from __future__ import division
from __future__ import print_function
from __future__ import unicode_literals

from hilma import Mesh, Polyline, spline, savePly
import numpy as np

polylineA = Polyline( [[100, 0], [100, 100], [0, 100], [0, 0]] );
splineA = spline(polylineA, 10.0);

polylineB = Polyline( np.array([[5, 25], [75, 75], [25, 75], [25, 25]], dtype=np.float32) );
splineB = spline(polylineB, 10.0);

mesh = Mesh();
mesh.append( splineA )
mesh.append( splineB )

savePly("spline.ply", mesh, False)