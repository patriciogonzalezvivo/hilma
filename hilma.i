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
    #include "hilma/BoundingBox.h"
    #include "hilma/compute.h"
    #include "hilma/generate.h"
    #include "hilma/Material.h"
    #include "hilma/math.h"
    #include "hilma/Mesh.h"
    #include "hilma/Polyline.h"
    #include "hilma/transform.h"
    #include "hilma/io/obj.h"
    #include "hilma/io/ply.h"
    #include "hilma/io/tinyply.h"
    #include "hilma/io/tiny_obj_loader.h"
%}

%include "numpy.i"
%init %{
    import_array();
%}

%apply (int* IN_ARRAY1, int DIM1 ) {(const int* _array1D, int _n )};
%apply (int* IN_ARRAY2, int DIM1, int DIM2 ) {(const int* _array2D, int _m, int _n )};

%apply (float* IN_ARRAY1, int DIM1 ) {(const float* _array1D, int _n )};
%apply (float* IN_ARRAY2, int DIM1, int DIM2 ) {(const float* _array2D, int _m, int _n )};

%include "include/hilma/Mesh.h"
%include "include/hilma/Material.h"
%include "include/hilma/BoundingBox.h"
%include "include/hilma/Polyline.h"
%include "include/hilma/transform.h"
%include "include/hilma/generate.h"
%include "include/hilma/compute.h"
%include "include/hilma/io/ply.h"
// %include "include/hilma/io/obj.h"

using namespace hilma;

