#ifndef PYRAMIDWORKS_GEOMETRY_RECT_H_
#define PYRAMIDWORKS_GEOMETRY_RECT_H_

#include <pyramidworks/geometry/geometricshape.h>

namespace pyramidworks {
namespace geometry {

/// \class Rect rect.h "pyramidworks/geometry/rect.h"
/// A rect, one type of GeometricShape.
class Rect : public GeometricShape {
  public:
    /** @param width  The rect width. 
      * @param height The rect height. */
    Rect(double _width, double _height) : GeometricShape(), half_width_(_width/2), half_height_(_height/2) {}
    
    /// Returns the rect width.
    /** @return A double. */
    double width() const { return 2*half_width_; }

    /// Returns the rect height.
    /** @return A double. */
    double height() const { return 2*half_height_; }

    bool Intersects(const ugdk::math::Vector2D& this_pos, const GeometricShape *obj, const ugdk::math::Vector2D& that_pos) const;
    bool Intersects(const ugdk::math::Vector2D& this_pos, const           Rect *obj, const ugdk::math::Vector2D& that_pos) const;
    bool Intersects(const ugdk::math::Vector2D& this_pos, const         Circle *obj, const ugdk::math::Vector2D& that_pos) const;
	bool Intersects(const ugdk::math::Vector2D& this_pos, const  ConvexPolygon *obj, const ugdk::math::Vector2D& that_pos) const;
    ugdk::structure::ikdtree::Box<2> GetBoundingBox(const ugdk::math::Vector2D& position) const;

  private:
    double half_width_, half_height_;
};

} // namespace geometry
} // namespace pyramidworks


#endif /* PYRAMIDWORKS_GEOMETRY_RECT_H_ */
