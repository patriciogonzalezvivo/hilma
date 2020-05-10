#include "hilma/io/stl.h"

#include <stdio.h>
#include <iostream>

namespace hilma {

bool saveStl( const std::string& _filename, const Mesh& _mesh, bool _binnary ) {
    std::vector<Triangles> triangles = _mesh.getTrianglesIndices();

    if (!_binnary) {

        
        FILE * stl_file = fopen(_filename.c_str(),"w");
        if(stl_file == NULL) {
            std::cerr <<"IOError: " << _filename << " could not be opened for writing."<< std::endl;
            return false;
        }

        fprintf(stl_file,"solid %s\n", _filename.c_str());
        for (size_t i = 0; i < triangles.size(); i++) {
            fprintf(stl_file,"facet normal %e %e %e\n",
                    (float)triangles[i].getNormal().x,
                    (float)triangles[i].getNormal().y,
                    (float)triangles[i].getNormal().z);

            fprintf(stl_file,"outer loop\n");
            for (int v = 0; v < 3; v++) {
                fprintf(stl_file,"vertex %e %e %e\n",
                                (float)triangles[i][v].x,
                                (float)triangles[i][v].y,
                                (float)triangles[i][v].z);
            }
            fprintf(stl_file,"endloop\n");
            fprintf(stl_file,"endfacet\n");
        }
        fprintf(stl_file,"endsolid %s\n", _filename.c_str());
        fclose(stl_file);
        return true;
    }
    else {
        FILE * stl_file = fopen(_filename.c_str(),"wb");
        if(stl_file == NULL) {
            std::cerr << "IOError: " << _filename << " could not be opened for writing." << std::endl;
            return false;
        }

        // Write unused 80-char header
        for (char h = 0;h<80;h++)
            fwrite(&h,sizeof(char), 1, stl_file);

        // Write number of triangles
        unsigned int num_tri = triangles.size();
        fwrite(&num_tri, sizeof(unsigned int), 1, stl_file);
        // assert(F.cols() == 3);

        // Write each triangle
        for (size_t i = 0; i < triangles.size(); i++) {
            fwrite(&triangles[i].getNormal().x, sizeof(float), 3, stl_file);
            for (int v = 0; v < 3; v++)
                fwrite(&triangles[i][v].x, sizeof(float), 3, stl_file);

            unsigned short att_count = 0;
            fwrite(&att_count, sizeof(unsigned short), 1, stl_file);
        }
        fclose(stl_file);
        return true;
    }
}

}
