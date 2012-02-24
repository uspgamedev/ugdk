
%module ugdk_action

%include <module/export.swig>
%include "std_vector.i"

%{

#include <ugdk/action/animation.h>
#include <ugdk/action/animationset.h>
#include <ugdk/action/animationframe.h>
#include <ugdk/action/entity.h>
#include <ugdk/action/observer.h>
#include <ugdk/action/scene.h>

%}

%template(AnimationFrameVector) std::vector<ugdk::AnimationFrame*>;

%import <ugdk/base/types.h>
%import <ugdk/graphic.h>
%import <module/audio.i>

%include <ugdk/action/animationframe.h>
%include <ugdk/action/animation.h>
%include <ugdk/action/animationset.h>
%include <ugdk/action/entity.h>
%include <ugdk/action/observer.h>
%include <ugdk/action/scene.h>

namespace ugdk {
    export_class(Animation)
    export_class(AnimationFrame)
    export_class(AnimationSet)
    export_class(AnimationManager)
    export_class(Entity)
    export_class(Observer)
    export_class(Scene)
}

confirm_exports(ugdk_action)
 
