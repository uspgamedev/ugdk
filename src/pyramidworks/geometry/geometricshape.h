#ifndef PYRAMIDWORKS_GEOMETRY_GEOMETRYOBJECT_H_
#define PYRAMIDWORKS_GEOMETRY_GEOMETRYOBJECT_H_

#include <ugdk/math/vector2D.h>
#include <ugdk/structure.h>
#include <pyramidworks/geometry.h>

namespace pyramidworks {
namespace geometry {

/// \class GeometricShape geometricshape.h "pyramidworks/geometry/geometricshape.h"
/// A geometric shape.
class GeometricShape {
  public:
    virtual ~GeometricShape() {}

    /// Checks if this shape intersects another shape.
    /** @param this_pos The absolute position for this shape.
        @param obj The shape to compare with.
        @param that_pos The absolute position of the other shape.
        @return True if there's an intersection. */
    virtual bool Intersects(const ugdk::math::Vector2D& this_pos, const GeometricShape *obj, const ugdk::math::Vector2D& that_pos) const = 0;

    /// Checks if there's an intersection with a Circle.
    /** @param this_pos The absolute position for this shape.
        @param obj The shape to compare with.
        @param that_pos The absolute position of the other shape.
        @return True if there's an intersection. */
    virtual bool Intersects(const ugdk::math::Vector2D& this_pos, const Circle *obj, const ugdk::math::Vector2D& that_pos) const = 0;

    /// Checks if there's an intersection with a Rect.
    /** @param this_pos The absolute position for this shape.
        @param obj The shape to compare with.
        @param that_pos The absolute position of the other shape.
        @return True if there's an intersection. */
    virtual bool Intersects(const ugdk::math::Vector2D& this_pos, const Rect *obj, const ugdk::math::Vector2D& that_pos) const = 0;

	/// Checks if there's an intersection with a Convex Polygon.
	/** @param this_pos The absolute position for this shape.
        @param obj The shape to compare with.
        @param that_pos The absolute position of the other shape.
        @return True if there's an intersection. */
	virtual bool Intersects(const ugdk::math::Vector2D& this_pos, const ConvexPolygon *obj, const ugdk::math::Vector2D& that_pos) const = 0;
    
    /// Returns a bounding box for this shape.
    /** The box is at the shape's position and offset
        @param position The absolute position for this shape.
        @return A 2D Box around this shape */
    virtual ugdk::structure::Box<2> GetBoundingBox(const ugdk::math::Vector2D& position) const = 0;

  protected:
    /** @param offset An offset to the position. Defaults to (0,0) */
    GeometricShape() {}
};

} // namespace geometry
} // namespace pyramidworks

#endif /* PYRAMIDWORKS_GEOMETRY_GEOMETRYOBJECT_H_ */
