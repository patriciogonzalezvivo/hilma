#pragma once

// #include "glm/glm.hpp"
#include "glm/fwd.hpp"
#include <vector>
#include <deque>

#include "Mesh.h"

namespace hilma {

enum class CapType {
    BUTT = 0,   // No points added to end of line
    SQUARE = 2, // Two points added to make a square extension
    ROUND = 6   // Six points added in a fan to make a round cap
};

enum class JoinType {
    MITER = 0,  // No points added at line join
    BEVEL = 1,  // One point added to flatten the corner of a join
    ROUND = 5   // Five points added in a fan to make a round outer join
};

template<class T>
class Polyline_ {
public:
    /// \name Constructors
    /// \{

    /// \brief Creates an Polyline.
    Polyline_();

    /// \brief Creates an Polyline from a vector of T objects.
    Polyline_(const std::vector<T>& verts);

    /// \brief Creates an Polyline from a 2D array of floats (NumPy compatibility).
    Polyline_(const float* _array2D, int _m, int _n);


    /// \}
    /// \name Add and Remove Vertices
    /// \{

    /// \brief Removes all the points from the Polyline.
    void clear();

    /// \brief Adds a point using an T at the end of the Polyline.
    void addVertex( const T& p );

    /// \brief Adds a point using an array of floats at the end of the Polyline.
    void addVertex(const float* _array1D, int _n);

    /// \brief Adds a point using floats at the end of the Polyline.
    void addVertex( float x, float y, float z=0 );

    /// \brief Add multiple points at the end of the Polyline using a vector of
    /// T objects
    ///
    /// ~~~~{.cpp}
    ///     // make a pentagon
    ///     float size = 80.f;
    ///     float X1 = 0.125*sqrt(10 + 2*sqrt(5)) * size;
    ///     float X2 = 0.125*sqrt(10 - 2*sqrt(5)) * size;
    ///     float Y1 = 0.125*(sqrt(5) - 1) * size;
    ///     float Y2 = 0.125*(sqrt(5) + 1) * size;
    ///     vector<T> verts = {
    ///         {0, -0.5 * size, 0.f),
    ///         {-X1, -Y1, 0.f},
    ///         {-X2, Y2, 0.f},
    ///         {X2, Y2, 0.f},
    ///         {X1, -Y1, 0.f},
    ///     };
    ///     Polyline p;
    ///     p.addVertices(verts);
    /// ~~~~
    void addVertices( const std::vector<T>& verts );

    /// \brief Adds multiple points at the end of the Polyline using a pointer to
    /// an array of T objects.
    void addVertices(const T* verts, int numverts);

    void addVertices(const float* _array2D, int _m, int _n);

    void insertVertex(const T &p, int index);
    void insertVertex(float x, float y, float z, int index);

    void append(const Polyline_<T>& _poly );


    /// \brief Remove a vertex at a given index.
    ///
    /// This function print an error and ignore the input if the index is
    /// invalid. When a vertex is removed, the internal caches are cleared.
    ///
    /// \param index The index of the vertex to remove.
    void removeVertex(int index);

    /// \brief Resize the number of points in the Polyline to the value
    /// passed in.
    void resize(size_t size);

    /// \}
    /// \name Access Vertices
    /// \{

    /// \brief The number of points in the Polyline.
    size_t size() const;

    /// Allows you to access the points of the Polyline just like you would
    /// in an array, so to make the points of a line follow the mouse
    /// movement, you could do:
    ///
    /// ~~~~{.cpp}
    /// line[0].set(mouseX, mouseY);
    /// int i = 1;
    /// while ( i<bounds.size()) {
    ///     float angle = atan2(line[i-1].y - line[i].y, line[i-1].x - line[i].x);
    ///     bounds[i].set(bounds[i-1].x - cos(angle) * 20, bounds[i-1].y - sin(angle) * 20);
    ///     i++;
    /// }
    /// ~~~~
    const T& operator[] (int index) const;
    T& operator[] (int index);

    /// \brief Gets a vector of vertices that the line contains
    std::vector<T> & getVertices();
    const std::vector<T> & getVertices() const;

    typename std::vector<T>::iterator begin();
    typename std::vector<T>::const_iterator begin() const;
    typename std::vector<T>::reverse_iterator rbegin();
    typename std::vector<T>::const_reverse_iterator rbegin() const;
    typename std::vector<T>::iterator end();
    typename std::vector<T>::const_iterator end() const;
    typename std::vector<T>::reverse_iterator rend();
    typename std::vector<T>::const_reverse_iterator rend() const;

    /// \}
    /// \name Lines and Curves
    /// \{

    /// \brief Add a straight line from the last point added, or from 0,0 if no point
    /// is set, to the point indicated by the T passesd in.
    void lineTo(const T & to ){ addVertex(to); }

    /// \brief Add a straight line from the last point added, or from 0,0 if no point
    /// is set, to the point indicated by the floats x,y,z passesd in.
    void lineTo(float x, float y, float z=0){
        addVertex(x,y,z);
    }

    /// \brief Adds an arc around the T `center` with the width of `radiusX`
    /// and the height of `radiusY` to the polyline.
    ///
    /// The `angleBegin` and `angleEnd` indicate the start and end angles of
    /// the arc in degrees measured clockwise from the x-axis.
    ///
    /// The `clockwise` boolean sets the drawing direction.  Passing 'false' to
    /// it will draw the arc counter-clockwise.
    ///
    /// Optionally, you can specify `circleResolution`, which is the number of
    /// line segments a circle would be drawn with.
    ///
    /// If the arc doesn't start at the same point the last vertex finished a
    /// straight line will be created to join both
    void arc(const T & center, float radiusX, float radiusY, float angleBegin, float angleEnd, bool clockwise, int circleResolution = 20);

    /// \brief Adds an arc around the T `center` with the width of
    /// `radiusX` and the height of `radiusY`.
    ///
    /// The `angleBegin` and `angleEnd` indicate the start and end angles
    /// of the arc in degrees measured clockwise from the x-axis.
    ///
    /// Optionally, you can specify `circleResolution`, which is the number of
    /// line segments a circle would be drawn with. A partial arc will be
    /// drawn with the same resolution: if circleResolution == 20, a half-
    /// circle will be drawn with 10 segments.
    ///
    /// If there are already vertexes in the Polyline the arc will extend
    /// them; a line will be created from the endmost point on the Polyline
    /// to the beginning point of the arc.
    ///
    /// ~~~~{.cpp}
    /// Polyline polyline1, polyline2;
    ///
    /// // draw an line, then an semi-circle in red
    /// polyline2.lineTo(300, 50);
    /// T point2(450,120);
    /// polyline2.arc(point2,100,100,0,180);
    /// ofSetColor(ofColor::red);
    /// polyline2.draw();
    /// ~~~~
    ///
    /// ![Arc Example](graphics/polyline_arc.jpg)
    void arc(const T & center, float radiusX, float radiusY, float angleBegin, float angleEnd, int circleResolution = 20) {
        arc(center, radiusX,  radiusY,  angleBegin,  angleEnd, true,  circleResolution);
    }

    /// \brief Adds an arc around the coordinates (`x`,`y`) with the width of
    /// `radiusX` and the height of `radiusY`.
    ///
    /// The `angleBegin` and `angleEnd` indicate the start and end angles
    /// of the arc in degrees measured clockwise from the x-axis.
    ///
    /// Optionally, you can specify `circleResolution`, which is the number
    /// of line segments a circle would be drawn with.
    void arc(float x, float y, float radiusX, float radiusY, float angleBegin, float angleEnd, int circleResolution = 20){
        arc(T(x, y, 0.f), radiusX, radiusY, angleBegin, angleEnd, true, circleResolution);
    }

    /// \brief Adds an arc around the coordinates (`x`,`y`,`z`) with the width of
    /// `radiusX` and the height of `radiusY`.
    ///
    /// The `angleBegin` and `angleEnd` indicate the start and end angles of
    /// the arc in degrees measured clockwise from the x-axis.
    ///
    /// Optionally, you can specify `circleResolution`, which is the number of
    /// line segments a circle would be drawn with.
    void arc(float x, float y, float z, float radiusX, float radiusY, float angleBegin, float angleEnd, int circleResolution = 20){
        arc(T(x, y, z), radiusX, radiusY, angleBegin, angleEnd, true, circleResolution);
    }
    void arcNegative(const T & center, float radiusX, float radiusY, float angleBegin, float angleEnd, int circleResolution = 20) {
        arc(center, radiusX, radiusY, angleBegin, angleEnd, false, circleResolution);
    }
    void arcNegative(float x, float y, float radiusX, float radiusY, float angleBegin, float angleEnd, int circleResolution = 20){
        arc(T(x,y,0.f), radiusX, radiusY, angleBegin, angleEnd, false, circleResolution);
    }
    void arcNegative(float x, float y, float z, float radiusX, float radiusY, float angleBegin, float angleEnd, int circleResolution = 20){
        arc(T(x, y, z), radiusX, radiusY, angleBegin, angleEnd, false, circleResolution);
    }



    /// \brief Adds a curve to an T object passed in
    ///
    /// ~~~~{.cpp}
    /// float angle = 0;
    /// while (angle < TWO_PI ) {
    ///     b.curveTo( T(100*cos(angle), 100*sin(angle)));
    ///     b.curveTo( T(300*cos(angle), 300*sin(angle)));
    ///     angle += TWO_PI / 30;
    /// }
    /// ~~~~
    ///
    /// \note You need at least 4 points to be able to use curveTo()
    /// \sa [Catmull-Rom splines wiki](http://en.wikipedia.org/wiki/Centripetal_Catmull%E2%80%93Rom_spline)
    void curveTo( const T & to, int curveResolution = 20 );

    /// \brief Adds a curve to the x,y,z points passed in with the optional
    /// resolution.
    void curveTo(float x, float y, float z = 0,  int curveResolution = 20 ){
        curveTo({x,y,z},curveResolution);
    }

    /// \brief Adds a cubic bezier line from the current drawing point with the 2
    /// control points indicated by T cp1 and cp2, that ends at T
    /// to.
    ///
    /// ~~~~{.cpp}
    /// line.addVertex(T(200, 400));
    /// line.bezierTo(100, 100, 800, 100, 700, 400);
    /// ~~~~
    /// ![polyline bezier](bezier.jpg)
    /// The control points are shown in red.
    void bezierTo( const T & cp1, const T & cp2, const T & to, int curveResolution = 20);

    /// \brief Adds a cubic bezier line from the current drawing point with the 2
    /// control points indicated by the coordinates cx1, cy1 and cx2, cy2,
    /// that ends at the coordinates x, y.
    void bezierTo(float cx1, float cy1, float cx2, float cy2, float x, float y, int curveResolution = 20){
        bezierTo({cx1,cy1,0.f}, {cx2,cy2,0.f}, {x,y,0.f}, curveResolution);
    }

    /// \brief Adds a cubic bezier line in 3D space from the current drawing point
    /// with the 2 control points indicated by the coordinates cx1, cy1, cz1
    /// and cx2, cy2, cz2, that ends at the coordinates x, y, z.
    void bezierTo(float cx1, float cy1, float cz1, float cx2, float cy2, float cz2, float x, float y, float z, int curveResolution = 20){
        bezierTo({cx1,cy1,cz1}, {cx2,cy2,cz2}, {x,y,z}, curveResolution);
    }

    /// \brief Adds a quadratic bezier line in 3D space from the current drawing
    /// point with the beginning indicated by the coordinates cx1, cy1, cz1,
    /// the control point at cx2, cy2, cz2, and that ends at the coordinates
    /// x, y, z.
    ///
    /// ![polyline curves](graphics/curves.png)
    void quadBezierTo(float cx1, float cy1, float cz1, float cx2, float cy2, float cz2, float x, float y, float z, int curveResolution = 20);

    /// \brief Adds a quadratic bezier line in 2D space from the current drawing
    /// point with the beginning indicated by the point p1, the control point
    /// at p2, and that ends at the point p3.
    void quadBezierTo(  const T & p1, const T & p2, const T & p3,  int curveResolution = 20 ){
        quadBezierTo(p1.x,p1.y,p1.z,p2.x,p2.y,p2.z,p3.x,p3.y,p3.z,curveResolution);
    }

    /// \brief Adds a quadratic bezier line in 2D space from the current drawing
    /// point with the beginning indicated by the coordinates cx1, cy1, the
    /// control point at cx2, cy2, and that ends at the coordinates x, y.
    void quadBezierTo(float cx1, float cy1, float cx2, float cy2, float x, float y, int curveResolution = 20){
        quadBezierTo(cx1,cy1,0,cx2,cy2,0,x,y,0,curveResolution);
    }

    /// \}
    /// \name Smoothing and Resampling
    /// \{

    /// \brief Gets a smoothed version of the Polyline.
    ///
    /// `smoothingSize` is the size of the smoothing window. So if
    /// `smoothingSize` is 2, then 2 points from the left, 1 in the center,
    /// and 2 on the right (5 total) will be used for smoothing each point.
    /// `smoothingShape` describes whether to use a triangular window (0) or
    /// box window (1) or something in between (for example, .5).
    Polyline_ getSmoothed(int smoothingSize, float smoothingShape = 0) const;

    /// \brief Resamples the line based on the spacing passed in. The larger the
    /// spacing, the more points will be eliminated.
    ///
    /// ~~~~{.cpp}
    /// line.draw();
    /// ofTranslate(400, 0);
    /// line.getResampledBySpacing(100).draw();
    /// ~~~~
    /// ![polyline resample](graphics/resample.jpg)
    Polyline_ getResampledBySpacing(float spacing) const;

    /// \brief Resamples the line based on the count passed in. The lower the
    /// count passed in, the more points will be eliminated.
    ///
    /// This doesn't add new points to the line.
    Polyline_ getResampledByCount(int count) const;

    /// \brief Simplifies the polyline, removing un-necessary vertices.
    ///
    /// \param tolerance determines how dis-similar points need to be to stay in the line.
    /// Higher tolerance means more points removed, lower tolerance means less
    /// points removed.
    void simplify(float tolerance=0.3f);

    /// \}
    /// \name Transform polyline
    /// \{

    void rotateDeg(float degrees, const glm::vec3& axis);
    void rotateRad(float radians, const glm::vec3& axis);

    void translate(const glm::vec3 & p);

    void rotateDeg(float degrees, const glm::vec2& axis);
    void rotateRad(float radians, const glm::vec2& axis);

    void translate(const glm::vec2 & p);

    /// \brief Change the size of the Polyline
    /// These changes are non-reversible, so for instance
    /// scaling by 0,0 zeros out all data.
    void scale(float x, float y);

    /// \}
    /// \name Polyline State
    /// \{

    /// \brief Closes the Polyline, meaning that all the vertices will be linked
    /// and can be "walked".
    void close();

    /// \brief Closes the Polyline, meaning that all the vertices will be linked
    /// and can be "walked".
    void setClosed( bool tf );
    bool isClosed() const;


    /// \brief Returns whether the vertices within the line have changed.
    bool hasChanged();
    void flagHasChanged();

    /// \}
    /// \name Geometric Functions
    /// \{

    /// \brief Tests whether the x,y coordinates are within a closed Polyline.
    static bool inside(float x, float y, const Polyline_ & polyline);
    /// \brief Tests whether the x,y coordinates are within a closed Polyline.
    bool inside(float x, float y) const;

    /// \brief Tests whether the T is within a closed Polyline.
    static bool inside(const T & p, const Polyline_ & polyline);
    /// \brief Tests whether the T is within a closed Polyline.
    bool inside(const T & p) const;

    /// \brief Gets the size of the perimeter of the polyline, good for
    /// determining length of the line, rather than just the bounding box
    /// shape.
    float getPerimeter() const;

    /// \brief Gets the point on the line closest to the target. You can also
    /// optionally pass a pointer to/address of an unsigned int to get the
    /// index of the closest vertex
    T getClosestPoint(const T& target, unsigned int* nearestIndex = nullptr) const;


    /// \}
    /// \name Other Functions
    /// \{

    /// \brief Get (interpolated) index at given length along the path
    ///
    /// Includes info on percentage along segment, e.g. `ret=5.75` =>
    /// 75% along the path between 5th and 6th points
    float getIndexAtLength(float f) const;

    /// \brief Get (interpolated) index at given percentage along the path
    ///
    /// Includes info on percentage along segment, e.g. `ret=5.75`
    /// => 75% along the path between 5th and 6th points
    float getIndexAtPercent(float f) const;

    /// \brief Get length along path at index
    float getLengthAtIndex(int index) const;

    /// \brief Get length along path at interpolated index (e.g. `f=5.75` => 75% along
    /// the path between 5th and 6th points)
    float getLengthAtIndexInterpolated(float findex) const;

    /// \brief Get point long the path at a given length (e.g. `f=150` => 150
    /// units along the path)
    T getPointAtLength(float f) const;

    /// \brief Get point along the path at a given percentage (e.g. `f=0.25`
    /// => 25% along the path)
    T getPointAtPercent(float f) const;

    /// \brief Get point along the path at interpolated index (e.g. `f=5.75` =>
    /// 75% along the path between 5th and 6th points)
    T getPointAtIndexInterpolated(float findex) const;

    /// \brief Get rotation vector at index (magnitude is sine of angle)
    T getRotationAtIndex(int index) const;

    /// \brief Get rotation vector at interpolated index
    /// (interpolated between neighboring indices) (magnitude is sine of angle)
    T getRotationAtIndexInterpolated(float findex) const;

    /// \brief Get angle (degrees) of the path at index
    float getDegreesAtIndex(int index) const;

    /// \brief Get angle (degrees) at interpolated index (interpolated between
    /// neighboring indices)
    float getDegreesAtIndexInterpolated(float findex) const;

    /// \brief Get angle (degrees) of the path at index
    float getRadiansAtIndex(int index) const;

    /// \brief Get angle (degrees) at interpolated index (interpolated between
    /// neighboring indices)
    float getRadiansAtIndexInterpolated(float findex) const;

    /// \brief Get tangent vector at index
    T getTangentAtIndex(int index) const;

    /// \brief Get tangent vector at interpolated index
    /// (interpolated between neighboring indices)
    T getTangentAtIndexInterpolated(float findex) const;

    /// \brief Get normal vector at index
    T getNormalAtIndex(int index) const;

    /// \brief Get normal vector at interpolated index
    /// (interpolated between neighboring indices)
    T getNormalAtIndexInterpolated(float findex) const;

    /// \brief Get wrapped index depending on whether poly is closed or not
    int getWrappedIndex(int index) const;

    // used for calculating the normals
    void setRightVector(T v = T(0, 0, -1));
    T getRightVector() const;

private:
    void setCircleResolution(int res);
    float wrapAngle(float angleRad);

    std::vector<T> points;
    T rightVector;

    // cache
    mutable std::vector<float> lengths;    // cumulative lengths, stored per point (lengths[n] is the distance to the n'th point, zero based)
    mutable std::vector<T> tangents;       // tangent at vertex, stored per point
    mutable std::vector<T> normals;        //
    mutable std::vector<T> rotations;      // rotation axes between adjacent segments, stored per point (cross product)
    mutable std::vector<float> angles;     // angle (rad) between adjacent segments, stored per point (asin(cross product))

    std::deque<T> curveVertices;
    std::vector<T> circlePoints;

    bool bClosed;
    bool bHasChanged;   // public API has access to this
    mutable bool bCacheIsDirty;   // used only internally, no public API to read

    void updateCache(bool bForceUpdate = false) const;

    // given an interpolated index (e.g. 5.75) return neighboring indices and interolation factor (e.g. 5, 6, 0.75)
    void getInterpolationParams(float findex, int &i1, int &i2, float &t) const;

    void calcData(int index, T &tangent, float &angle, T &rotation, T &normal) const;

};

using Polyline = Polyline_<glm::vec3>;

// /// \brief Determine if an (x,y) coordinate is within the polygon defined by a vector of glm::vec3s.
// /// \param x The x dimension of the coordinate.
// /// \param y The y dimension of the coordinate.
// /// \param polygon a vector of glm::vec3s defining a polygon.
// /// \returns True if the point defined by the coordinates is enclosed, false otherwise.
// template<class T>
// bool ofInsidePoly(float x, float y, const std::vector<T>& polygon){
//  return Polyline_<T>::inside(x,y, Polyline_<T>(polygon));
// }


// /// \brief Determine if an glm::vec3 is within the polygon defined by a vector of glm::vec3s.
// /// \param p A point to check.
// /// \param poly A vector of glm::vec3s defining a polygon.
// /// \returns True if the glm::vec3 is enclosed, false otherwise.
// template<class T>
// bool ofInsidePoly(const T& p, const std::vector<T>& poly){
//  return Polyline_<T>::inside(p.x,p.y, Polyline_<T>(poly));

// }

}