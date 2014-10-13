
#include <pyramidworks/geometry/circle.h>
#include <pyramidworks/geometry/rect.h>
#include <pyramidworks/geometry/convexpolygon.h>
#include <ugdk/structure/intervalkdtree.h>

namespace pyramidworks {
namespace geometry {

using namespace ugdk;

bool Circle::Intersects (const ugdk::math::Vector2D& this_pos, const Rect *rect, const ugdk::math::Vector2D& that_pos) const {
    return rect->Intersects(that_pos, this, this_pos);
}

bool Circle::Intersects (const ugdk::math::Vector2D& this_pos, const Circle *circle, const ugdk::math::Vector2D& that_pos) const {
    ugdk::math::Vector2D distance = that_pos - this_pos;
    return distance.length() <= circle->radius_ + this->radius_;
}

bool Circle::Intersects (const ugdk::math::Vector2D& this_pos, const GeometricShape *obj, const ugdk::math::Vector2D& that_pos) const {
    return obj->Intersects(that_pos, this, this_pos);
}

bool Circle::Intersects(const ugdk::math::Vector2D& this_pos, const  ConvexPolygon *obj, const ugdk::math::Vector2D& that_pos) const {
	return obj->Intersects(that_pos, this, this_pos);
}

ugdk::structure::Box<2> Circle::GetBoundingBox (const ugdk::math::Vector2D& thispos) const {
    ugdk::math::Vector2D thisposmin(thispos.x - radius_,
                        thispos.y - radius_);
    ugdk::math::Vector2D thisposmax(thispos.x + radius_,
                        thispos.y + radius_);
    return ugdk::structure::Box<2>(thisposmin, thisposmax);
}
} // namespace geometry
} // namespace pyramidworks
