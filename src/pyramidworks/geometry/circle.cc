
#include "circle.h"
#include "rect.h"
#include <ugdk/util/intervalkdtree.h>

namespace pyramidworks {
namespace geometry {

using namespace ugdk;

bool Circle::Intersects (const ugdk::Vector2D& this_pos, const Rect *rect, const ugdk::Vector2D& that_pos) const {
    return rect->Intersects(that_pos, this, this_pos);
}

bool Circle::Intersects (const ugdk::Vector2D& this_pos, const Circle *circle, const ugdk::Vector2D& that_pos) const {
    Vector2D distance = that_pos - this_pos;
    return distance.length() <= circle->radius_ + this->radius_;
}

bool Circle::Intersects (const ugdk::Vector2D& this_pos, const GeometricShape *obj, const ugdk::Vector2D& that_pos) const {
    return obj->Intersects(that_pos, this, this_pos);
}

ugdk::ikdtree::Box<2> Circle::GetBoundingBox (const ugdk::Vector2D& thispos) const {
    Vector2D thisposmin(thispos.x - radius_,
                        thispos.y - radius_);
    Vector2D thisposmax(thispos.x + radius_,
                        thispos.y + radius_);
    return ugdk::ikdtree::Box<2>(thisposmin.val, thisposmax.val);
}
} // namespace geometry
} // namespace pyramidworks
