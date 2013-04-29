
%module ugdk_drawable

%include <module/export.swig>
%include <module/proxy.swig>
%include <module/widetypes.swig>
%include "std_string.i"
%include "std_vector.i"

%{

#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/graphic/drawable/label.h>
#include <ugdk/graphic/drawable/textbox.h>
#include <ugdk/graphic/drawable/texturedrectangle.h>

#include <module/ugdk/action/observerproxy.h>

%}

%import(module="ugdk_math") <ugdk/math/vector2D.h>
%import(module="ugdk_base") <ugdk/base/types.h>
%import(module="ugdk_action") <ugdk/action.h>
%import(module="ugdk_action") <ugdk/action/spritetypes.h>
%import(module="ugdk_graphic") <ugdk/graphic/geometry.h>
%import(module="ugdk_graphic") <ugdk/graphic/visualeffect.h>
%import(module="ugdk_graphic") <ugdk/graphic/drawable.h>
%import(module="ugdk_graphic") <ugdk/graphic/spritesheet.h>
%import(module="ugdk_graphic") <ugdk/graphic/texture.h>
%import(module="ugdk_graphic") <ugdk/graphic/font.h>

%ignore ugdk::graphic::Text::SetMessage(const std::vector<std::wstring>& message);

proxy_class(ugdk::action::Observer)

%include <ugdk/graphic/drawable/sprite.h>
%include <ugdk/graphic/drawable/texturedrectangle.h>
%include <ugdk/graphic/drawable/label.h>
%include <ugdk/graphic/drawable/textbox.h>


namespace ugdk {
namespace graphic {
    export_class(Sprite)
    export_class(TexturedRectangle)
    export_class(Label)
    export_class(TextBox)
}
}

confirm_exports(ugdk_drawable)
