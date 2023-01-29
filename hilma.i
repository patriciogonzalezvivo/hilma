%begin %{
#if defined( __WIN32__ ) || defined( _WIN32 )
	#include <cmath>
#endif
%}

%module hilma

%include <typemaps.i>
%include <std_vector.i>
%include <std_string.i>

%ignore *::operator[];
%ignore *::operator=;
%ignore *::operator==;
%ignore *::operator!=;
%ignore *::operator<;
%ignore *::operator<=;
%ignore *::operator>;
%ignore *::operator>=;
%ignore operator<<;

%{
    #define SWIG_FILE_WITH_INIT
    #include "hilma/ops/math.h"
    #include "hilma/types/ray.h"
    #include "hilma/types/line.h"
    #include "hilma/types/image.h"
    #include "hilma/types/material.h"
    #include "hilma/types/triangle.h"
    #include "hilma/types/plane.h"
    #include "hilma/types/mesh.h"
    #include "hilma/types/polyline.h"
    #include "hilma/types/polygon.h"
    #include "hilma/types/camera.h"
    #include "hilma/types/boundingBox.h"
    #include "hilma/types/boundingSphere.h"
    #include "hilma/types/bvh.h"
    #include "hilma/types/kdtree.h"
    #include "hilma/ops/compute.h"
    #include "hilma/ops/generate.h"
    #include "hilma/ops/intersection.h"
    #include "hilma/ops/image.h"
    #include "hilma/ops/path.h"
    #include "hilma/ops/transform.h"
    #include "hilma/ops/raytrace.h"
    #include "hilma/io/jpg.h"
    #include "hilma/io/png.h"
    #include "hilma/io/hdr.h"
    #include "hilma/io/ply.h"
    #include "hilma/io/stl.h"
    #include "hilma/io/obj.h"
    #include "hilma/io/gltf.h"
    #include "hilma/io/auto.h"
%}

%include "glm.i"
%include "numpy.i"
%init %{
    import_array();
%}

%typemap(in) size_t {
    $1 = PyInt_AsLong($input);
}

%typemap(in) uint16_t {
    $1 = PyInt_AsLong($input);
}

%typemap(in) uint32_t {
    $1 = PyInt_AsLong($input);
}

%apply (int* IN_ARRAY1, int DIM1 ) {(const int* _array1D, int _n )};
%apply (int* IN_ARRAY2, int DIM1, int DIM2 ) {(const int* _array2D, int _m, int _n )};

%apply (uint16_t* IN_ARRAY1, int DIM1 ) {(const uint16_t* _array1D, int _n )};
%apply (uint16_t* IN_ARRAY2, int DIM1, int DIM2 ) {(const uint16_t* _array2D, int _m, int _n )};

%apply (uint32_t* IN_ARRAY1, int DIM1 ) {(const uint32_t* _array1D, int _n )};
%apply (uint32_t* IN_ARRAY2, int DIM1, int DIM2 ) {(const uint32_t* _array2D, int _m, int _n )};

%apply (float* IN_ARRAY1, int DIM1 ) {(const float* _array1D, int _n )};
%apply (float* IN_ARRAY2, int DIM1, int DIM2 ) {(const float* _array2D, int _m, int _n )};

%apply (uint8_t* IN_ARRAY3, int DIM1, int DIM2, int DIM3) { (const uint8_t* _array3D, int _height, int _width, int _channels) }
%apply (uint8_t** ARGOUTVIEWM_ARRAY3, int* DIM1, int* DIM2, int* DIM3) { (uint8_t **_array3D, int *_height, int *_width, int *_channels) }

namespace std {
    %template(Vector4DVector)   vector<glm::vec4>;
    %template(Vector3DVector)   vector<glm::vec3>;
    %template(Vector2DVector)   vector<glm::vec2>;

    %template(PolylinesVector)  vector<hilma::Polyline>;

    %template(FacesVector)      vector<glm::ivec3>;
    %template(EdgesVector)      vector<glm::ivec2>;
    %template(TrianglesVector)  vector<hilma::Triangle>;
    %template(LinesVector)      vector<hilma::Line>;
    %template(MeshesVector)     vector<hilma::Mesh>;

    %template(ImagesVector)     vector<hilma::Image>;

    %template(StringVector)     vector<string>;
    %template(FloatVector)      vector<float>;
};

%include "include/hilma/types/ray.h"
%include "include/hilma/types/line.h"
%include "include/hilma/types/image.h"
%include "include/hilma/types/material.h"
%include "include/hilma/types/triangle.h"
%include "include/hilma/types/plane.h"
%include "include/hilma/types/mesh.h"
%include "include/hilma/types/polyline.h"
%include "include/hilma/types/polygon.h"
%include "include/hilma/types/camera.h"
%include "include/hilma/types/boundingBox.h"
%include "include/hilma/types/boundingSphere.h"
%include "include/hilma/types/bvh.h"
%include "include/hilma/types/kdtree.h"
%include "include/hilma/ops/intersection.h"
%include "include/hilma/ops/image.h"
%include "include/hilma/ops/path.h"
%include "include/hilma/ops/compute.h"
%include "include/hilma/ops/generate.h"
%include "include/hilma/ops/raytrace.h"
%include "include/hilma/ops/transform.h"
%include "include/hilma/io/jpg.h"
%include "include/hilma/io/png.h"
%include "include/hilma/io/hdr.h"
%include "include/hilma/io/ply.h"
%include "include/hilma/io/stl.h"
%include "include/hilma/io/obj.h"
%include "include/hilma/io/gltf.h"
%include "include/hilma/io/auto.h"

// using namespace hilma;


