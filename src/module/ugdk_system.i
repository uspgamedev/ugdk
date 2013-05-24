%module ugdk_system

%include <module/export.swig>
%include <module/ownership.swig>
%include <module/proxy.swig>
%include "std_string.i"
%include "std_map.i"

%{

#include <ugdk/common/types.h>
#include <ugdk/system/configuration.h>
#include <ugdk/system/engine.h>

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

%include <ugdk/common/types.h>
%include <ugdk/system/configuration.h>
%include <ugdk/system/engine.h>

namespace ugdk {
	export_class(Color)
	export_class(Configuration)
	export_class(Engine)
}
 
confirm_exports(ugdk_system)
