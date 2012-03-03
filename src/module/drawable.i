
%module ugdk_drawable

%include <module/export.swig>
%include "std_string.i"
%include "std_vector.i"

%{

#include <ugdk/graphic/drawable/shape.h>
#include <ugdk/graphic/drawable/solidrectangle.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/graphic/drawable/text.h>
#include <ugdk/graphic/drawable/texturedrectangle.h>

%}

%import <module/math.i>
%import <module/base.i>
%import <module/action.i>
%import <module/graphic.i>

%include <ugdk/graphic/drawable/shape.h>
%include <ugdk/graphic/drawable/solidrectangle.h>
%include <ugdk/graphic/drawable/sprite.h>
%include <ugdk/graphic/drawable/text.h>
%include <ugdk/graphic/drawable/texturedrectangle.h>


namespace ugdk {
namespace graphic {
    export_class(Shape)
    export_class(Text)
    export_class(Sprite)
    export_class(TexturedRectangle)
    export_class(SolidRectangle)
}
}

confirm_exports(ugdk_drawable)
