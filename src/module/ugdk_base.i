%module ugdk_base

%include <module/export.swig>
%include <module/ownership.swig>
%include "std_string.i"
%include "std_map.i"

%{

#include <ugdk/base/types.h>
#include <ugdk/base/configuration.h>
#include <ugdk/base/engine.h>
#include <ugdk/base/resourcecontainer.h> //class template
//#include <ugdk/base/genericcontainer.h>
#include <ugdk/base/resourcemanager.h>

#include <ugdk/action/animationset.h>
#include <ugdk/graphic/texture.h>
#include <ugdk/graphic/spritesheet.h>
#include <ugdk/util/languageword.h>

%}

%include <ugdk/base/resourcecontainer.h> //class template

enable_disown(ugdk::graphic::Texture* val)
%template(ResourceContainer_Texture) ugdk::base::ResourceContainer<ugdk::graphic::Texture*>;
disable_disown(ugdk::graphic::Texture* val)

enable_disown(ugdk::AnimationSet* val)
%template(ResourceContainer_AnimationSet) ugdk::base::ResourceContainer<ugdk::AnimationSet*>;
disable_disown(ugdk::AnimationSet* val)

enable_disown(ugdk::graphic::Spritesheet* val)
%template(ResourceContainer_Spritesheet) ugdk::base::ResourceContainer<ugdk::graphic::Spritesheet*>;
disable_disown(ugdk::graphic::Spritesheet* val)

enable_disown(ugdk::LanguageWord* val)
%template(ResourceContainer_LanguageWord) ugdk::base::ResourceContainer<ugdk::LanguageWord*>;
disable_disown(ugdk::LanguageWord* val)

%import(module="ugdk_math") <ugdk/math/vector2D.h>
%import(module="ugdk_input") <ugdk/input.h>
%import(module="ugdk_time") <ugdk/time.h>
%import(module="ugdk_audio") <ugdk/audio.h>
%import(module="ugdk_action") <ugdk/action.h>
%import(module="ugdk_graphic") <ugdk/graphic.h>
%import(module="ugdk_util") <ugdk/util.h>

%include <ugdk/base/types.h>
%include <ugdk/base/configuration.h>
%include <ugdk/base/engine.h>
%include <ugdk/base/resourcemanager.h>

namespace ugdk {
	export_class(Color)
	export_class(Configuration)
	export_class(Engine)
	
namespace base {
    export_class(ResourceManager)
}
}
 
confirm_exports(ugdk_base)
