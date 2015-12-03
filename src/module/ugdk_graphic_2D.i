
%module ugdk_graphic

%include <module/util_ugdk.swig>

%include "std_string.i"
%include "std_vector.i"
%include "std_map.i"

#define final

%{

#include <ugdk/graphic/light.h>
#include <ugdk/graphic/drawmode.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/textureatlas.h>
#include <ugdk/graphic/textureunit.h>
#include <ugdk/graphic/rendertarget.h>
#include <ugdk/graphic/rendertexture.h>
#include <ugdk/graphic/manager.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/vertexdata.h>
#include <ugdk/graphic/primitive.h>
#include <ugdk/graphic/shader.h>
#include <ugdk/graphic/shaderprogram.h>
#include "graphic/gltexture.h"

%}

%import(module="ugdk_math") <ugdk/math/vector2D.h>
%import(module="ugdk_math") <ugdk/math/frame.h>
%import(module="ugdk_desktop") <ugdk/desktop.h>

%ignore ugdk::graphic::SpritesheetData::frames() const;

%include <ugdk/graphic.h>

%import(module="ugdk_system") <ugdk/structure/types.h>
%import(module="ugdk_action") <ugdk/action.h>
%import(module="ugdk_graphic") <ugdk/graphic.h>
%import(module="ugdk_structure") <ugdk/structure.h>

%include <ugdk/graphic/drawmode.h>
%include <ugdk/graphic/canvas.h>
%include <ugdk/graphic/light.h>
%include <ugdk/graphic/textureatlas.h>
%include <ugdk/graphic/textureunit.h>
%include <ugdk/graphic/rendertarget.h>
%include <ugdk/graphic/rendertexture.h>
%include <ugdk/graphic/shader.h>
%include <ugdk/graphic/shaderprogram.h>
%include <ugdk/structure/indexabletable.h>

%include <ugdk/graphic/vertexdata.h>
%include <ugdk/graphic/primitive.h>

%include <ugdk/graphic/manager.h>
%include <ugdk/graphic/module.h>

namespace ugdk {
namespace graphic {
	export_class(DrawMode)
    export_class(Canvas)
    export_class(Light)
    export_class(TextureAtlas)
	export_class(TextureUnit)
	export_class(RenderTarget)
	export_class(RenderTexture)
    export_class(Manager)
    export_class(Primitive)
    export_class(VertexData)
	export_class(Shader)
	export_class(ShaderProgram)
}
}

confirm_exports(ugdk_graphic)
