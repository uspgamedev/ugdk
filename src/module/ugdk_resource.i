%module ugdk_resource

%include <module/util_ugdk.swig>

%include "std_string.i"
%include "std_map.i"

%{

#include <ugdk/resource/resourcecontainer.h> //class template
//#include <ugdk/resource/genericcontainer.h>
#include <ugdk/resource/manager.h>
#include <ugdk/resource/module.h>

//#include <ugdk/action/animationset.h>
#include <ugdk/graphic/texture.h>
#include <ugdk/graphic/spritesheet.h>
#include <ugdk/util/languageword.h>

%}


%include <ugdk/resource/resourcecontainer.h> //class template

enable_disown(ugdk::graphic::Texture* val)
%template(ResourceContainer_Texture) ugdk::resource::ResourceContainer<ugdk::graphic::Texture*>;
disable_disown(ugdk::graphic::Texture* val)

//enable_disown(ugdk::action::AnimationSet* val)
//%template(ResourceContainer_AnimationSet) ugdk::resource::ResourceContainer<ugdk::action::AnimationSet*>;
//disable_disown(ugdk::action::AnimationSet* val)

enable_disown(ugdk::graphic::Spritesheet* val)
%template(ResourceContainer_Spritesheet) ugdk::resource::ResourceContainer<ugdk::graphic::Spritesheet*>;
disable_disown(ugdk::graphic::Spritesheet* val)

enable_disown(ugdk::LanguageWord* val)
%template(ResourceContainer_LanguageWord) ugdk::resource::ResourceContainer<ugdk::LanguageWord*>;
disable_disown(ugdk::LanguageWord* val)

%import(module="ugdk_math") <ugdk/math/vector2D.h>
%import(module="ugdk_input") <ugdk/input.h>
%import(module="ugdk_time") <ugdk/time.h>
%import(module="ugdk_audio") <ugdk/audio.h>
%import(module="ugdk_action") <ugdk/action.h>
%import(module="ugdk_graphic") <ugdk/graphic.h>
%import(module="ugdk_util") <ugdk/util.h>

proxy_class(ugdk::action::Scene)

%include <ugdk/resource/manager.h>
%include <ugdk/resource/module.h>

namespace ugdk {
namespace resource {
    export_class(Manager)
}
}
 
confirm_exports(ugdk_resource)
