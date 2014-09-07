
%module ugdk_action

%include <module/util_ugdk.swig>

%include "std_vector.i"
%include "std_string.i"

%{

#include <ugdk/action/animationplayer.h>
#include <ugdk/action/mediamanager.h>
#include <ugdk/action/mediaplayer.h>
#include <ugdk/action/observer.h>
#include <ugdk/action/scene.h>
#include <ugdk/action/spriteanimationframe.h>
#include <ugdk/action/spritetypes.h>

// Proxies
#include <module/ugdk/action/observerproxy.h>
#include <module/ugdk/action/sceneproxy.h>

%}

%import(module="ugdk_structure") <ugdk/structure.h>
%import(module="ugdk_system") <ugdk/system.h>
%import(module="ugdk_system") <ugdk/structure/types.h>
%import(module="ugdk_graphic") <ugdk/graphic.h>
%import(module="ugdk_audio") <ugdk/audio.h>
%import(module="ugdk_action") <ugdk/action.h>
%import(module="ugdk_util") <ugdk/util.h>
%import(module="ugdk_util") <ugdk/util/uncopyable.h>

proxy_class(ugdk::action::Observer)
proxy_class(ugdk::action::Scene)

%include <ugdk/system/taskplayer.h>
%include <ugdk/action/mediaplayer.h>
%include <ugdk/action/observer.h>
%include <ugdk/action/scene.h>
%include <ugdk/action/animationplayer.h>
%include <ugdk/action/spriteanimationframe.h>
%include <ugdk/action/spritetypes.h>
%include <ugdk/structure/indexabletable.h>

%template(SpriteAnimationPlayer) ugdk::action::AnimationPlayer<ugdk::action::SpriteAnimationFrame>;
%template(SpriteAnimationTable) ugdk::structure::IndexableTable<std::vector<ugdk::action::SpriteAnimationFrame*>*>;

namespace ugdk {
namespace action { 
    export_class(MediaPlayer)
    export_class(Observer)
    export_class(Scene)
    export_class(SpriteAnimationFrame)
}
}

confirm_exports(ugdk_action)
 
