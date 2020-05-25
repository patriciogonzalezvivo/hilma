#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from __future__ import absolute_import
from __future__ import division
from __future__ import print_function
from __future__ import unicode_literals

from hilma import Image, loadPng, savePng, toLuma, sdf, toSdf, sqrt, toNormalMap

# in_img = Image()
# loadPng("input.png", in_img, 1)
# print( in_img.getChannels() )
# gray = toLuma(img)
# out_img = toSdf(in_img)
# sdf(in_img)
# savePng("output.png", in_img)

heightmap = Image()
loadPng("gale.png", heightmap, 1)
normalmap = toNormalMap(heightmap, 1.0)
savePng("normalmap.png", normalmap)
