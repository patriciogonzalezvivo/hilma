#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from __future__ import absolute_import
from __future__ import division
from __future__ import print_function
from __future__ import unicode_literals

from hilma import Image, load, save, gamma, invert, autolevel, threshold, flip, toLuma, toSdf, toNormalmap

import cv2

# load from OpenCV
data = cv2.imread("earth.jpg")
img = Image(data)

# Load using hilma loaders
# img = Image()
# load("earth.jpg", img)

img_invert = Image(img)
invert(img_invert)
save("img_invert.png", img_invert)

img_gamma = Image(img)
gamma(img_gamma, 1.0/2.2)
save("img_gamma.png", img_gamma)

img_autolevel = Image(img)
autolevel(img_autolevel)
save("img_autolevel.png", img_autolevel)

img_threshold = Image(img)
threshold(img_threshold, 0.5)
save("img_threshold.png", img_threshold)

img_flip = Image(img)
flip(img_flip)
save("img_flip.png", img_flip)

luma = toLuma(img)
save("img_luma.png", luma)

luma_threshold = Image(luma)
threshold(luma_threshold, 0.5)
save("img_luma_threshold.png", luma_threshold)

luma_sdf = toSdf(luma_threshold)
save("img_lumma_sdf.png", luma_sdf)

heightmap = Image()
load("gale.png", heightmap, 1)
normalmap = toNormalmap(heightmap)
save("normalmap.png", normalmap)
    