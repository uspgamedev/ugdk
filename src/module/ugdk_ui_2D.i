
%module ugdk_ui

%include <module/util_ugdk.swig>

%include "std_string.i"
%include "std_vector.i"
%include "std_map.i"

%{

#include <ugdk/structure/intervalkdtree.h>

#include <ugdk/ui/hookpoint.h>
#include <ugdk/ui/drawable.h>
#include <ugdk/ui/node.h>
#include <ugdk/ui/uielement.h>
#include <ugdk/ui/button.h>
#include <ugdk/ui/menu.h>

#include <ugdk/ui/drawable/texturedrectangle.h>
#include <ugdk/ui/drawable/primitive.h>

%}

%import(module="ugdk_graphic") <ugdk/graphic.h>
%import(module="ugdk_math") <ugdk/math.h>
%import(module="ugdk_structure") <ugdk/structure.h>

%include <ugdk/ui.h>
%include <ugdk/ui/hookpoint.h>
%include <ugdk/ui/drawable.h>
%include <ugdk/ui/node.h>
%include <ugdk/ui/uielement.h>
%include <ugdk/ui/button.h>
%include <ugdk/ui/menu.h>

%include <ugdk/ui/drawable/texturedrectangle.h>
%include <ugdk/ui/drawable/primitive.h>

namespace ugdk {
namespace ui {
	export_class(Button)
	export_class(Menu)
	export_class(UIElement)
	export_class(Node)
	export_class(Drawable)
	export_class(DrawablePrimitive)
	export_class(TexturedRectangle)
}
}

confirm_exports(ugdk_ui)
