
%module ugdk_math

%include <module/export.swig>

%{

#include <ugdk/math/vector2D.h>
#include <ugdk/math/frame.h>

%}

%include <ugdk/math/vector2D.h>
%include <ugdk/math/frame.h>

namespace ugdk {
    export_class(Vector2D)
    export_class(Frame)
}

confirm_exports(ugdk_math)

