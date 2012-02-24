%module ugdk_base

%include <module/export.swig>
%include "std_string.i"

%{

#include <ugdk/base/types.h>
#include <ugdk/base/configuration.h>
#include <ugdk/base/engine.h>
#include <ugdk/base/resourcecontainer.h> //class template
#include <ugdk/base/texturecontainer.h>
#include <ugdk/base/resourcemanager.h>

#include <ugdk/graphic/texture.h>

%}


%import <module/math.i>
%import <module/input.i>
%import <module/time.i>
%import <module/audio.i>
%import <module/action.i>
%import <ugdk/graphic.h>

//%ignore ugdk::operator*(const double  &scalar, const Vector2D &right);

%newobject ugdk::base::TextureContainet::Load(const std::string& filepath, const std::string& tag);
%newobject ugdk::base::TextureContainet::Load(const std::string& filepath);

%include <ugdk/base/types.h>
%include <ugdk/base/configuration.h>
%include <ugdk/base/engine.h>
%include <ugdk/base/resourcecontainer.h> //class template
%template(ResourceContainer_Texture) ugdk::base::ResourceContainer<ugdk::graphic::Texture*>;
%include <ugdk/base/texturecontainer.h>
%include <ugdk/base/resourcemanager.h>

namespace ugdk {
	export_class(Color)
	export_class(Configuration)
	export_class(Engine)
	
namespace base {
    export_class(TextureContainer)
    export_class(ResourceManager)
}
}
 
confirm_exports(ugdk_base)
