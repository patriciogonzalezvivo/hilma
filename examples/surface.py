#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from __future__ import absolute_import
from __future__ import division
from __future__ import print_function
from __future__ import unicode_literals

from hilma import Mesh, surface, savePly
import numpy

polygon.push_back({{100, 0}, {100, 100}, {0, 100}, {0, 0}});
polygon.push_back({{75, 25}, {75, 75}, {25, 75}, {25, 25}});


mesh = surface(polygon);

savePly("points_plane.ply", mesh, False)