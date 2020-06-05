#include "hilma/types/Polygon.h"

namespace hilma {

Polygon::Polygon() {
}

Polygon::Polygon( const std::vector< Polyline >& _polygon ) {
    data = _polygon;
}

Polygon::Polygon( const std::vector< std::vector<glm::vec2> >& _polygon ) {
    append( _polygon );
}

Polygon::Polygon( const std::vector< std::vector<glm::vec3> >& _polygon ) {
    append( _polygon );
}

void Polygon::append( const Polyline& _poly ) {
    data.push_back(_poly);
}

void Polygon::append( const Polygon& _poly ) {
    for (size_t i = 0; i < _poly.size(); i++)
        data.push_back(_poly[i]);
}

void Polygon::append( const std::vector< std::vector<glm::vec2> >& _polygon ) {
    for (size_t i = 0; i < _polygon.size(); i++)
        append( Polyline(_polygon[i]) );
}

void Polygon::append( const std::vector< std::vector<glm::vec3> >& _polygon ) {
    for (size_t i = 0; i < _polygon.size(); i++)
        append( Polyline(_polygon[i]) );
}

}