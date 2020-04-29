
%module hypatia
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

%apply double &OUTPUT { double &_x, double &_y };
%apply double &OUTPUT { double &_lng, double &_lat };

%{
    #define SWIG_FILE_WITH_INIT
    
    #include "hilma/Mesh.h"
    #include "hilma/PolarPoint.h"
    #include "hilma/Polyline.h"
    #include "hilma/Rectangle.h"
    #include "hilma/io/fs.h"
    #include "hilma/io/obj.h"
    #include "hilma/io/ply.h"
    
%}

%include "include/hilma/Mesh.h"
%include "include/hilma/PolarPoint.h"
%include "include/hilma/Polyline.h"
%include "include/hilma/Rectangle.h"
%include "include/hilma/io/fs.h"
%include "include/hilma/io/obj.h"
%include "include/hilma/io/ply.h"

namespace std {
    %template(VectorInt) vector<int>;
    %template(VectorEquatorial) vector<Equatorial>;
};
