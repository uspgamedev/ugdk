%module ugdk_util

%include <module/export.swig>
%include "std_map.i"
%include "std_string.i"

%{

#include <ugdk/util/uncopyable.h>
#include <ugdk/util/languageword.h>
#include <ugdk/util/language.h>
#include <ugdk/util/languagemanager.h>

%}

%import(module="ugdk_action") <ugdk/action.h>
%import(module="ugdk_graphic") <ugdk/graphic.h>

%include <ugdk/util/uncopyable.h>
%include <ugdk/util/languageword.h>
%include <ugdk/util/language.h>
%include <ugdk/util/languagemanager.h>

namespace ugdk {
	export_class(LanguageWord)
	export_class(Language)
	export_class(LanguageManager)
	
namespace util {
    export_class(Uncopyable)
}
}
 
confirm_exports(ugdk_util)
