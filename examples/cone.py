#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from __future__ import absolute_import
from __future__ import division
from __future__ import print_function
from __future__ import unicode_literals

from hilma import Mesh, cone, savePly

mesh = cone(1., 2., 36, 1, 1)
savePly("cone.ply", mesh, False)
