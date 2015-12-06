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

enable_disown(ugdk::graphic::GLTexture* val)
%template(ResourceContainer_GLTexture) ugdk::resource::ResourceContainer<ugdk::graphic::GLTexture*>;
disable_disown(ugdk::graphic::GLTexture* val)

enable_disown(ugdk::graphic::TextureAtlas* val)
%template(ResourceContainer_TextureAtlas) ugdk::resource::ResourceContainer<ugdk::graphic::TextureAtlas*>;
disable_disown(ugdk::graphic::TextureAtlas* val)

enable_disown(ugdk::text::LanguageWord* val)
%template(ResourceContainer_LanguageWord) ugdk::resource::ResourceContainer<ugdk::text::LanguageWord*>;
disable_disown(ugdk::text::LanguageWord* val)

%import(module="ugdk_action") <ugdk/action.h>
%import(module="ugdk_action") <ugdk/action/spritetypes.h>
%import(module="ugdk_graphic") <ugdk/graphic.h>
%import(module="ugdk_text") <ugdk/text.h>

proxy_class(ugdk::action::Scene)

// SWIG fails really hard at wrapping this.
%ignore AddContainer(std::type_index type, std::unique_ptr<ResourceContainerBase> container);

%include <ugdk/resource/manager.h>
%include <ugdk/resource/module.h>

namespace ugdk {
namespace resource {
    export_class(Manager)
}
}
 
confirm_exports(ugdk_resource)
