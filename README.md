
Hilma is a library that:
- simple C++ interfaces
- minimal dependences so the C++ code can be added to other C++ projects easily
- add Python package which makes a C++ wrapper usign swig

## Hilma as C++ lib

You can easily add hypatia into your project by adding the `include/` and `src/` folder or by installing hypatia into your system. For the former option you need to:

```bash
sudo apt install cmake
mkdir build
cd build
cmake ..
make
sudo make install
```

```cpp
#include "hilma/Mesh.h"
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

**Note**: to install inside anaconda do:
```bash
/anaconda3/bin/./python3.7 setup.py install 
```

Then you can use it as follow:

```python

from hilma import Mesh, savePly

mesh = Mesh()
mesh.addVertices( [ [-1.0, -1.0], [-1.0, 1.0], [1.0, 1.0], [1.0, -1.0] ] )
mesh.addIndices( [( 1, 0, 3 ), (1, 3, 2)] ) 
savePly("rect.ply", mesh, False)

```

# License

Hilma is open source. You may use, extend, and redistribute without charge under the terms of the [BSD license](LICENSE).

```
Copyright (c) 2020, Patricio Gonzalez Vivo
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this
list of conditions and the following disclaimer in the documentation and/or other
materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
THE POSSIBILITY OF SUCH DAMAGE.
```