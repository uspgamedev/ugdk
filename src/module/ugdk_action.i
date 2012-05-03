
%module ugdk_action

%include <module/export.swig>
%include <module/proxy.swig>
%include "std_vector.i"

%{

#include <ugdk/action/animation.h>
#include <ugdk/action/animationset.h>
#include <ugdk/action/animationframe.h>
#include <ugdk/action/task.h>
#include <ugdk/action/entity.h>
#include <ugdk/action/observer.h>
#include <ugdk/action/scene.h>

#include <module/ugdk/action/observerproxy.h>
#include <module/ugdk/action/entityproxy.h>
#include <module/ugdk/action/sceneproxy.h>

%}

%template(AnimationFrameVector) std::vector<ugdk::action::AnimationFrame*>;

%import(module="ugdk_base") <ugdk/base/types.h>
%import(module="ugdk_graphic") <ugdk/graphic.h>
%import(module="ugdk_audio") <ugdk/audio.h>
%import(module="ugdk_action") <ugdk/action.h>

proxy_class(ugdk::action::Observer)
proxy_class(ugdk::action::Entity)
proxy_class(ugdk::action::Scene)

%ignore ugdk::action::Scene::content_node() const;
%ignore ugdk::action::Scene::interface_node() const;

%include <ugdk/action/animationframe.h>
%include <ugdk/action/animation.h>
%include <ugdk/action/animationset.h>
%include <ugdk/action/task.h>
%include <ugdk/action/entity.h>
%include <ugdk/action/observer.h>
%include <ugdk/action/scene.h>

namespace ugdk {
namespace action { 
    export_class(Animation)
    export_class(AnimationFrame)
    export_class(AnimationSet)
    export_class(AnimationManager)
    export_class(Entity)
    export_class(Observer)
    export_class(Scene)
    export_class(Task)
}
}

confirm_exports(ugdk_action)
 
