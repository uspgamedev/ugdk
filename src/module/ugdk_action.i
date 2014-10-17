
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

// Proxies
#include <module/ugdk/action/observerproxy.h>
#include <module/ugdk/action/sceneproxy.h>

%}

%import(module="ugdk_audio") <ugdk/audio.h>
%import(module="ugdk_structure") <ugdk/structure.h>
%import(module="ugdk_system") <ugdk/system/task.h>
%import(module="ugdk_graphic") <ugdk/graphic.h>

%include <ugdk/action.h>

proxy_class(ugdk::action::Observer)
proxy_class(ugdk::action::Scene)

%include <ugdk/system/taskplayer.h>
%include <ugdk/action/mediaplayer.h>
%include <ugdk/action/observer.h>
%include <ugdk/action/scene.h>
%include <ugdk/action/animationplayer.h>

namespace ugdk {
namespace action { 
    export_class(MediaPlayer)
    export_class(Observer)
    export_class(Scene)
}
}

confirm_exports(ugdk_action)
 
