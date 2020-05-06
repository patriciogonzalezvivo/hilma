#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from __future__ import absolute_import
from __future__ import division
from __future__ import print_function
from __future__ import unicode_literals

from hilma import Mesh, Polyline, tube, vec3, savePly
from math import radians, cos, sin

polyline = Polyline()
for i in range(1000):
    a = radians(i * 0.5)
    polyline.addVertex(vec3(cos(a*1.2), sin(a*2.), -cos(a*3.2) * sin(a*1.5)) * 10.0)

mesh = tube(polyline, 1.0, 18)
savePly("tube.ply", mesh, False)