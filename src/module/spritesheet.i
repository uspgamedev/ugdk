
%module ugdk_spritesheet

%include <module/export.swig>
%include "std_string.i"
%include "std_vector.i"

%{

#include <ugdk/graphic/spritesheet/fixedspritesheet.h>
#include <ugdk/graphic/spritesheet/flexiblespritesheet.h>

%}

%import <module/math.i>
%import <module/base.i>
%import <module/graphic.i>

%ignore ugdk::graphic::FixedSpritesheetData::SpritesheetFrame;
%ignore ugdk::graphic::FixedSpritesheetData::frames() const;

%newobject ugdk::graphic::FlexibleSpritesheet::render_size() const;

%include <ugdk/graphic/spritesheet/fixedspritesheet.h>
%include <ugdk/graphic/spritesheet/flexiblespritesheet.h>


namespace ugdk {
namespace graphic {
    export_class(FixedSpritesheetData)
    export_class(FixedSpritesheet)
    export_class(FlexibleSpritesheet)
}
}

confirm_exports(ugdk_spritesheet)
