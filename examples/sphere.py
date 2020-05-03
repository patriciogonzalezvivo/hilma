#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from __future__ import absolute_import
from __future__ import division
from __future__ import print_function
from __future__ import unicode_literals

from hilma import Mesh, sphere, savePly
import numpy

size = 1024

mesh = sphere(size/2, 10)
savePly("sphere.ply", mesh, False)