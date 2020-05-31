#include "hilma/types/Polygon.h"

namespace hilma {

Polygon::Polygon() {

}

Polygon::Polygon( const std::vector< Polyline >& _polygon ) {
    data = _polygon;
}

Polygon::Polygon( const std::vector< std::vector<glm::vec2> >& _polygon ) {
    for (size_t i = 0; i < _polygon.size(); i++)
        append( Polyline(_polygon[i]) );
}

Polygon::Polygon( const std::vector< std::vector<glm::vec3> >& _polygon ) {
    for (size_t i = 0; i < _polygon.size(); i++)
        append( Polyline(_polygon[i]) );
}


}