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

%include "include/hilma/Mesh.h"
%include "include/hilma/Material.h"
// %include "include/hilma/io/ObjOps.h"
%include "include/hilma/io/PlyOps.h"
//%include "include/hilma/io/tinyply.h"
//%include "include/hilma/io/tiny_obj_loader.h"

using namespace hilma;

