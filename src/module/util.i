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

%import <module/action.i>
%import <module/gdd.i>
%import <module/graphic.i>

%template(DescriptionProtocol_AnimationSet) ugdk::gdd::DescriptionProtocol<ugdk::AnimationSet>;

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
