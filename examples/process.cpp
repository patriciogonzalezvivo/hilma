#include <string>
#include <sstream>
#include <unistd.h>
#include <iostream>

#include "hilma/io/auto.h"
#include "hilma/ops/convert_image.h"

using namespace hilma;

int main(int argc, char **argv) {

    Image img;
    load("earth.jpg", img);
    
    Image img_gamma = Image(img);
    gamma(img_gamma, 2.2);
    save("img_gamma.png", img_gamma);

    Image img_invert = Image(img);
    invert(img_invert);
    save("img_invert.png", img_invert);

    Image img_autolevel = Image(img);
    autolevel(img_autolevel);
    save("img_autolevel.png", img_autolevel);

    Image img_threshold = Image(img);
    threshold(img_threshold, 0.5f);
    save("img_threshold.png", img_threshold);

    Image img_flip = Image(img);
    flip(img_flip);
    save("img_flip.png", img_flip);
    
    Image luma = toLuma(img);
    save("img_luma.png", luma);

    Image luma_threshold = Image(luma);
    threshold(luma_threshold, 0.5f);
    save("img_luma_threshold.png", luma_threshold);

    Image luma_sdf = toSdf(luma_threshold);
    save("img_lumma_sdf.png", luma_sdf);

    Image heightmap;
    load("gale.png", heightmap, 1);
    Image normalmap = toNormalmap(heightmap);
    save("normalmap.png", normalmap);
    
    return 1;
}
