#ifndef PYRAMIDWORKS_GEOMETRY_CIRCLE_H_
#define PYRAMIDWORKS_GEOMETRY_CIRCLE_H_

#include <pyramidworks/geometry/geometricshape.h>

namespace pyramidworks {
namespace geometry {

/// \class Circle circle.h "pyramidworks/geometry/circle.h"
/// A circle, one type of GeometricShape.
class Circle : public GeometricShape {
  public:
    /** @param radius The circle radius. */
    Circle(double _radius) : GeometricShape(), radius_(_radius) {}

    /// Getter for the radius.
    /** @return A double. */
    double radius() const { return radius_; }

    /// Setter the radius.
    /** @param radius The new radius. */
    void set_radius(double _radius) { radius_ = _radius; }

    bool Intersects(const ugdk::math::Vector2D& this_pos, const GeometricShape *obj, const ugdk::math::Vector2D& that_pos) const;
    bool Intersects(const ugdk::math::Vector2D& this_pos, const           Rect *obj, const ugdk::math::Vector2D& that_pos) const;
    bool Intersects(const ugdk::math::Vector2D& this_pos, const         Circle *obj, const ugdk::math::Vector2D& that_pos) const;
	bool Intersects(const ugdk::math::Vector2D& this_pos, const  ConvexPolygon *obj, const ugdk::math::Vector2D& that_pos) const;
    ugdk::structure::ikdtree::Box<2> GetBoundingBox(const ugdk::math::Vector2D& position) const;

  private:
    double radius_;
};

} // namespace geometry
} // namespace pyramidworks

#endif /* PYRAMIDWORKS_GEOMETRY_CIRCLE_H_ */
