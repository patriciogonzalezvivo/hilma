
![](docs/00.jpg)

# HILMA [![](https://img.shields.io/static/v1?label=Sponsor&message=%E2%9D%A4&logo=GitHub&color=%23fe8e86)](https://github.com/sponsors/patriciogonzalezvivo)


Hilma is 3D geometry library in C++ with Python bindings (with support for Numpy) ready to be embed into your projects. 

Dependencies:

* CMake, Swig and Python3
* [GLM](https://github.com/g-truc/glm) required
* [OpenImageDenoise](https://github.com/OpenImageDenoise/oidn) optional


## Compile and install Hilma

1. Before compiling install dependencies:

```bash
// In linux
sudo apt install libglm-dev cmake swig python3-numpy  

// In MacOS
brew install glm cmake swig
brew install --build-from-source open-image-denoise
```

2. Then compile

```bash
mkdir build
cd build
cmake ..
make
sudo make install 
```

**Note**: to install inside anaconda do:
```bash
swig -c++ -python hilma.i
/anaconda3/bin/./python3.7 setup.py build_ext --inplace
/anaconda3/bin/./python3.7 setup.py install 
```

## Hilma as C++ lib

Alternatively to installing Hilma on your system you can easily copy the content of `include/` and `src/` folder to your project, just make sure you 


```cpp
#include "hilma/types/Mesh.h"
#include "hilma/io/ply.h"

int main(int argc, char **argv) {

    float vertices[][2] = { {-1.0, -1.0} , {-1.0, 1.0} , {1.0, 1.0}, {1.0, -1.0} };
    int indices[][3] = { {1, 0, 3}, {1, 3, 2 } };

    hilma::Mesh mesh;
    mesh.addVertices( &vertices[0][0], 4, 2);
    mesh.addIndices( &indices[0][0], 2, 3);
    hilma::savePly("rect.ply", mesh, false);

    return 1;
}
```

## Hilma as Python module

First you need to create the package and install it

```bash
sudo apt install swig
make install
```

Then you can use it as follow:

```python

from hilma import Mesh, savePly

mesh = Mesh()
mesh.addVertices( [ [-1.0, -1.0], [-1.0, 1.0], [1.0, 1.0], [1.0, -1.0] ] )
mesh.addIndices( [( 1, 0, 3 ), (1, 3, 2)] ) 
savePly("rect.ply", mesh, False)

```

# Acknowledgements 

Some of the code is based or depends 3rd party libraries such us:

* [OpenFrameworks](https://openframeworks.cc/) `ofMesh` and `ofPolyline`
* [Mapbox's Earcut tessellator](https://github.com/mapbox/earcut)
* [Tangram-es's ployline to spline converter](https://github.com/tangrams/tangram-es/blob/master/core/src/util/builders.cpp)
* [Dan Wilcox's swig wrapper for GLM](https://github.com/danomatika/swig-openframeworks)
* [Dimitri Diakopoulos' TinyPly](https://github.com/ddiakopoulos/tinyply)
* [Syoyo Fujita's TinyGltf](https://github.com/syoyo/tinygltf)
* [Syoyo Fujita's TinyObj loader](https://github.com/tinyobjloader/tinyobjloader)
* [Peter Shirley's Ray Tracing in One Weekend ](https://raytracing.github.io/books/RayTracingInOneWeekend.html)
* [Ray/Triangle intersection](https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle)
* [Michael Fogleman's Heightmap meshing utility](https://github.com/fogleman/hmm)
* [Felzenszwalb page at Brown University for Image to SDF](https://cs.brown.edu/people/pfelzens/dt/)

## License 

HILMA is dual-licensed under [the Prosperity License](https://prosperitylicense.com/versions/3.0.0) and the [Patron License](https://patronlicense.com/versions/1.0.0.html) for [sponsors](https://github.com/sponsors/patriciogonzalezvivo) and [contributors](https://github.com/patriciogonzalezvivo/hilma/graphs/contributors).

[Sponsors](https://github.com/sponsors/patriciogonzalezvivo) and [contributors](https://github.com/patriciogonzalezvivo/lygia/graphs/contributors) will be added to the [Patron License](https://patronlicense.com/versions/1.0.0.html) and they can ignore the any non-commercial rule of [the Prosperity Licensed](https://prosperitylicense.com/versions/3.0.0).