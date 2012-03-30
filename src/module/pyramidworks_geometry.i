%module pyramidworks_geometry

%include <module/export.swig>

%{

#include <pyramidworks/geometry/geometricshape.h>
#include <pyramidworks/geometry/circle.h>
#include <pyramidworks/geometry/rect.h>

%}

%import(module="ugdk_math") <ugdk/math/vector2D.h>

%import <module/util.i>

%include <pyramidworks/geometry/geometricshape.h>
%include <pyramidworks/geometry/circle.h>
%include <pyramidworks/geometry/rect.h>

namespace pyramidworks {
namespace geometry {
    export_class(GeometricShape)
    export_class(Circle)
    export_class(Rect)
}
}
 
confirm_exports(ugdk_gdd)
