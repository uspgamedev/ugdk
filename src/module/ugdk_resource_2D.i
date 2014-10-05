%module ugdk_resource

%include <module/util_ugdk.swig>

%include "std_string.i"
%include "std_map.i"

%{

#include <ugdk/resource/resourcecontainer.h> //class template
#include <ugdk/resource/manager.h>
#include <ugdk/resource/module.h>

#include <ugdk/graphic/textureatlas.h>
#include <ugdk/text/languageword.h>

%}


%include <ugdk/resource/resourcecontainer.h> //class template

enable_disown(ugdk::internal::GLTexture* val)
%template(ResourceContainer_GLTexture) ugdk::resource::ResourceContainer<ugdk::internal::GLTexture*>;
disable_disown(ugdk::internal::GLTexture* val)

//enable_disown(ugdk::action::AnimationSet* val)
//%template(ResourceContainer_AnimationSet) ugdk::resource::ResourceContainer<ugdk::action::AnimationSet*>;
//disable_disown(ugdk::action::AnimationSet* val)

enable_disown(ugdk::graphic::TextureAtlas* val)
%template(ResourceContainer_TextureAtlas) ugdk::resource::ResourceContainer<ugdk::graphic::TextureAtlas*>;
disable_disown(ugdk::graphic::TextureAtlas* val)

enable_disown(ugdk::text::LanguageWord* val)
%template(ResourceContainer_LanguageWord) ugdk::resource::ResourceContainer<ugdk::text::LanguageWord*>;
disable_disown(ugdk::text::LanguageWord* val)

%import(module="ugdk_action") <ugdk/action.h>
%import(module="ugdk_graphic") <ugdk/graphic.h>
%import(module="ugdk_text") <ugdk/text.h>
%import(module="ugdk_internal") <ugdk/internal.h>

proxy_class(ugdk::action::Scene)

%include <ugdk/resource/manager.h>
%include <ugdk/resource/module.h>

namespace ugdk {
namespace resource {
    export_class(Manager)
}
}
 
confirm_exports(ugdk_resource)
