%module ugdk_system

%include <module/export.swig>
%include <module/ownership.swig>
%include <module/proxy.swig>
%include "std_string.i"
%include "std_map.i"

// Manually including cmath in the beginning prevents errors on some compilers
%begin %{
#include <cmath>
%}

%{

#include <ugdk/structure/types.h>
#include <ugdk/system/configuration.h>
#include <ugdk/system/engine.h>
#include <ugdk/system/task.h>
#include <ugdk/system/taskplayer.h>

//#include <ugdk/action/animationset.h>
#include <ugdk/graphic/texture.h>
#include <ugdk/graphic/spritesheet.h>
#include <ugdk/util/languageword.h>

#include <module/ugdk/action/sceneproxy.h>

%}


%import(module="ugdk_math") <ugdk/math/vector2D.h>
%import(module="ugdk_input") <ugdk/input.h>
%import(module="ugdk_time") <ugdk/time.h>
%import(module="ugdk_audio") <ugdk/audio.h>
%import(module="ugdk_action") <ugdk/action.h>
%import(module="ugdk_graphic") <ugdk/graphic.h>
%import(module="ugdk_util") <ugdk/util.h>
%import(module="ugdk_resource") <ugdk/resource.h>

proxy_class(ugdk::action::Scene)

%include <ugdk/system.h>
%include <ugdk/structure/types.h>
%include <ugdk/system/configuration.h>
%include <ugdk/system/engine.h>
%include <ugdk/system/taskplayer.h>

namespace ugdk {
	export_class(Color)

namespace system {
	export_class(Configuration)
	export_class(TaskPlayer)
} 
}
 
confirm_exports(ugdk_system)
