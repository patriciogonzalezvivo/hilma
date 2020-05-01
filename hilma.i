%module hilma
%include "typemaps.i"
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
    #include "hilma/Mesh.h"
    #include "hilma/Material.h"
    #include "hilma/io/ObjOps.h"
    #include "hilma/io/PlyOps.h"
    #include "hilma/io/tinyply.h"
    #include "hilma/io/tiny_obj_loader.h"
%}

%include "numpy.i"
%init %{
    import_array();
%}

%apply (int* IN_ARRAY1, int DIM1 ) {(const int* _data, int _n )};
%apply (float* IN_ARRAY1, int DIM1 ) {(const float* _data, int _n )};

%apply (int* IN_ARRAY2, int DIM1, int DIM2 ) {(const int* _data, int _m, int _n )};
%apply (float* IN_ARRAY2, int DIM1, int DIM2 ) {(const float* _data, int _m, int _n )};


%include "include/hilma/Mesh.h"
%include "include/hilma/Material.h"
%include "include/hilma/io/PlyOps.h"
// %include "include/hilma/io/ObjOps.h"

using namespace hilma;

