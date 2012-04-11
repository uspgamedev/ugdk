%module ugdk_util

%include <module/export.swig>
%include "std_map.i"
%include "std_string.i"

%{

#include <ugdk/util/animationparser.h>
#include <ugdk/util/animationprotocol.h>
#include <ugdk/util/pathmanager.h>
#include <ugdk/util/uncopyable.h>
#include <ugdk/util/languageword.h>
#include <ugdk/util/language.h>
#include <ugdk/util/languagemanager.h>
#include <ugdk/util/intervalkdtree.h>

%}

%import(module="ugdk_action") <ugdk/action.h>
%import(module="ugdk_gdd") <ugdk/util/gdd/descriptionprotocol.h>
%import(module="ugdk_gdd") <ugdk/util/gdd/abstractloader.h>
%import(module="ugdk_graphic") <ugdk/graphic.h>

%template(DescriptionProtocol_AnimationSet) ugdk::gdd::DescriptionProtocol<ugdk::action::AnimationSet>;

%include <ugdk/util/animationparser.h>
%include <ugdk/util/animationprotocol.h>
%include <ugdk/util/pathmanager.h>
%include <ugdk/util/uncopyable.h>
%include <ugdk/util/languageword.h>
%include <ugdk/util/language.h>
%include <ugdk/util/languagemanager.h>
%include <ugdk/util/intervalkdtree.h>


namespace ugdk {
	export_class(AnimationParser)
	export_class(PathManager)
	export_class(AnimationProtocol)
	export_class(LanguageWord)
	export_class(Language)
	export_class(LanguageManager)
	
namespace util {
    export_class(Uncopyable)
}
}
 
confirm_exports(ugdk_util)
