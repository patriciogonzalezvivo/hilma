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
    fprintf(obj_file,"# generated with Hilma by Patricio Gonzalez Vivo\n");
    fprintf(obj_file,"o %s\n", _mesh.getName().c_str());

    // TODO:
    //      - MATERIALS

    // Loop over V
    for(size_t i = 0; i < _mesh.vertices.size(); i++)
        fprintf(obj_file,"v %0.17g %0.17g %0.17g\n", _mesh.vertices[i].x, _mesh.vertices[i].y, _mesh.vertices[i].z); 
    // fprintf(obj_file,"\n");

    bool write_normals = _mesh.haveNormals();
    if (write_normals) {
        for (size_t i = 0; i < _mesh.normals.size(); i++)
            fprintf(obj_file,"vn %0.17g %0.17g %0.17g\n", _mesh.normals[i].x, _mesh.normals[i].y, _mesh.normals[i].z);
        // fprintf(obj_file,"\n");
    }

    bool write_texture_coords = _mesh.haveTexCoords();
    if (write_texture_coords) {
        for (size_t i = 0; i < _mesh.texcoords.size(); i++)
            fprintf(obj_file, "vt %0.17g %0.17g\n", _mesh.texcoords[i].x, _mesh.texcoords[i].y);
        // fprintf(obj_file,"\n");
    }

    if (_mesh.haveFaceIndices()) {
        std::vector<glm::ivec3> faces = _mesh.getTrianglesIndices();
        for (size_t i = 0; i < faces.size(); i++) {
            fprintf(obj_file,"f");

            for(int j = 0; j < 3; j++) {
                fprintf(obj_file," %u", faces[i][j] + 1);

                if(write_texture_coords)
                    fprintf(obj_file,"/%u", faces[i][j] + 1);
            
                if(write_normals) {
                    if (write_texture_coords)
                        fprintf(obj_file,"/%u", faces[i][j] + 1);
                    else
                        fprintf(obj_file,"//%u", faces[i][j] + 1);
                }
            }

            fprintf(obj_file,"\n");
        }
    }
    fclose(obj_file);

    return true;
};


}

