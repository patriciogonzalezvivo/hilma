#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from __future__ import absolute_import
from __future__ import division
from __future__ import print_function
from __future__ import unicode_literals

from hilma import Mesh, plane, savePly
import numpy

size = 1024

mesh = plane(size, size, 10, 10)
savePly("plane.ply", mesh, False)