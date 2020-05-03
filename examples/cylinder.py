#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from __future__ import absolute_import
from __future__ import division
from __future__ import print_function
from __future__ import unicode_literals

from hilma import Mesh, cylinder, savePly

mesh = cylinder(1.0, 2.0, 36, 1, 1, True)
savePly("cylinder.ply", mesh, False)
