#include <string>
#include <sstream>
#include <unistd.h>
#include <iostream>


#include "hilma/io/jpg.h"
#include "hilma/io/png.h"
#include "hilma/types/Image.h"
#include "hilma/ops/process.h"

using namespace hilma;

int main(int argc, char **argv) {

    Image img = loadJpg("earth.jpg");
    
    Image img_gamma = Image(img);
    gamma(img_gamma, 2.2);
    savePng("img_gamma.png", img_gamma);

    Image img_invert = Image(img);
    invert(img_invert);
    savePng("img_invert.png", img_invert);

    Image img_autolevel = Image(img);
    autolevel(img_autolevel);
    savePng("img_autolevel.png", img_autolevel);

    Image img_threshold = Image(img);
    threshold(img_threshold, 0.5f);
    savePng("img_threshold.png", img_threshold);

    Image img_flip = Image(img);
    flip(img_flip);
    savePng("img_flip.png", img_flip);
    
    Image luma = toLuma(img);
    savePng("img_luma.png", luma);

    Image luma_threshold = Image(luma);
    threshold(luma_threshold, 0.5f);
    savePng("img_luma_threshold.png", luma_threshold);

    Image luma_sdf = toSdf(luma_threshold);
    savePng("img_lumma_sdf.png", luma_sdf);

    Image heightmap = loadPng("gale.png", 1);
    Image normalmap = toNormalMap(heightmap, -10.0);
    savePng("normalmap.png", normalmap);
    
    return 1;
}