#include "hilma/io/obj.h"

#include <stdio.h>
#include <iostream>
#include <limits>
#include <iomanip>
#include <fstream>
#include <cstdio>
#include <cassert>

#ifndef HILMA_LINE_MAX
#define HILMA_LINE_MAX 2048
#endif

namespace hilma {

bool saveObj( const std::string& _filename, const Mesh& _mesh ) {
    FILE * obj_file = fopen(_filename.c_str(), "w");
    if (NULL==obj_file) {
        printf("IOError: %s could not be opened for writing...", _filename.c_str());
        return false;
    }

    // https://github.com/libigl/libigl/blob/master/include/igl/writeOBJ.cpp

    return true;
};


}

