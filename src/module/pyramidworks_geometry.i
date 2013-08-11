%module pyramidworks_geometry

%include <module/export.swig>
%include "std_vector.i"

%{

#include <ugdk/structures/intervalkdtree.h>

#include <pyramidworks/geometry.h>
#include <pyramidworks/geometry/geometricshape.h>
#include <pyramidworks/geometry/circle.h>
#include <pyramidworks/geometry/convexpolygon.h>
#include <pyramidworks/geometry/rect.h>

%}

%import(module="ugdk_math") <ugdk/math/vector2D.h>

%import <module/ugdk_util.i>

%newobject pyramidworks::geometry::Circle::GetBoundingBox(const ugdk::math::Vector2D& position) const;
%newobject pyramidworks::geometry::Rect::GetBoundingBox(const ugdk::math::Vector2D& position) const;
%newobject pyramidworks::geometry::ConvexPolygon::GetBoundingBox(const ugdk::math::Vector2D& position) const;

%include <pyramidworks/geometry.h>
%include <pyramidworks/geometry/geometricshape.h>
%include <pyramidworks/geometry/circle.h>
%include <pyramidworks/geometry/convexpolygon.h>
%include <pyramidworks/geometry/rect.h>

namespace pyramidworks {
namespace geometry {
    export_class(GeometricShape)
    export_class(Circle)
	export_class(ConvexPolygon)
    export_class(Rect)
}
}
 
confirm_exports(pyramidworks_geometry)
