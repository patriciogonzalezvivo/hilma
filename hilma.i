
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
    
    #include "include/hilma/*.h"
    
%}

%include "include/hilma/*.h"

namespace std {
    %template(VectorInt) vector<int>;
    %template(VectorEquatorial) vector<Equatorial>;
};
