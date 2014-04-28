
%module ugdk_drawable

%include <module/util_ugdk.swig>

%include "std_string.i"
%include "std_vector.i"

%{

#include <ugdk/graphic/drawable/texturedrectangle.h>
#include <ugdk/graphic/text/label.h>
#include <ugdk/graphic/text/textbox.h>

#include <module/ugdk/action/observerproxy.h>

%}

%import(module="ugdk_internal") <ugdk/internal.h>
%import(module="ugdk_math") <ugdk/math/vector2D.h>
%import(module="ugdk_system") <ugdk/structure/types.h>
%import(module="ugdk_action") <ugdk/action.h>
%import(module="ugdk_action") <ugdk/action/spritetypes.h>
%import(module="ugdk_graphic") <ugdk/graphic/canvas.h>
%import(module="ugdk_graphic") <ugdk/graphic/drawable.h>
%import(module="ugdk_graphic") <ugdk/graphic/spritesheet.h>
%import(module="ugdk_graphic") <ugdk/graphic/text/font.h>

%ignore ugdk::graphic::Text::SetMessage(const std::vector<std::wstring>& message);

proxy_class(ugdk::action::Observer)

%include <ugdk/graphic/drawable/texturedrectangle.h>
%include <ugdk/graphic/text/label.h>
%include <ugdk/graphic/text/textbox.h>


namespace ugdk {
namespace graphic {
    export_class(TexturedRectangle)
    export_class(Label)
    export_class(TextBox)
}
}

confirm_exports(ugdk_drawable)
