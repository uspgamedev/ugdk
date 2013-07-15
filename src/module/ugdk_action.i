
%module ugdk_action

%include <module/export.swig>
%include <module/proxy.swig>
%include "std_vector.i"
%include "std_string.i"

%{

#include <ugdk/action/mediaplayer.h>
#include <ugdk/action/entity.h>
#include <ugdk/action/observer.h>
#include <ugdk/action/scene.h>
#include <ugdk/action/animationplayer.h>
#include <ugdk/action/spriteanimationframe.h>
#include <ugdk/action/spritetypes.h>

#include <module/ugdk/action/observerproxy.h>
#include <module/ugdk/action/entityproxy.h>
#include <module/ugdk/action/sceneproxy.h>
#include <module/ugdk/action/taskproxy.h>

%}

%import(module="ugdk_base") <ugdk/common/types.h>
%import(module="ugdk_graphic") <ugdk/graphic.h>
%import(module="ugdk_audio") <ugdk/audio.h>
%import(module="ugdk_action") <ugdk/action.h>
%import(module="ugdk_util") <ugdk/util.h>
%import(module="ugdk_util") <ugdk/util/uncopyable.h>

proxy_class(ugdk::action::Observer)
proxy_class(ugdk::action::Entity)
proxy_class(ugdk::action::Scene)
proxy_class(ugdk::action::Task)

%ignore ugdk::action::Scene::content_node() const;
%ignore ugdk::action::Scene::interface_node() const;
%ignore ugdk::action::Entity::OnSceneAdd(Scene* scene);

%include <ugdk/action/mediaplayer.h>
%include <ugdk/action/entity.h>
%include <ugdk/action/observer.h>
%include <ugdk/action/scene.h>
%include <ugdk/action/animationplayer.h>
%include <ugdk/action/spriteanimationframe.h>
%include <ugdk/action/spritetypes.h>
%include <ugdk/util/indexabletable.h>

%template(SpriteAnimationPlayer) ugdk::action::AnimationPlayer<ugdk::action::SpriteAnimation>;
%template(SpriteAnimationTable) ugdk::util::IndexableTable<ugdk::action::SpriteAnimation*>;

namespace ugdk {
namespace action { 
    export_class(MediaPlayer)
    export_class(Entity)
    export_class(Observer)
    export_class(Scene)
    export_class(SpriteAnimation)
    export_class(SpriteAnimationFrame)
}
}

confirm_exports(ugdk_action)
 
