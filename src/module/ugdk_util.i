%module ugdk_util

%include <module/export.swig>
%include "std_map.i"
%include "std_string.i"

%{

#include <ugdk/util/pathmanager.h>
#include <ugdk/util/uncopyable.h>
#include <ugdk/util/languageword.h>
#include <ugdk/util/language.h>
#include <ugdk/util/languagemanager.h>
#include <ugdk/util/intervalkdtree.h>

%}

%import(module="ugdk_action") <ugdk/action.h>
%import(module="ugdk_gdd") <ugdk/util/gdd/descriptionprotocol.h>
%import(module="ugdk_gdd") <ugdk/util/gdd/loader.h>
%import(module="ugdk_graphic") <ugdk/graphic.h>

%include <ugdk/util/pathmanager.h>
%include <ugdk/util/uncopyable.h>
%include <ugdk/util/languageword.h>
%include <ugdk/util/language.h>
%include <ugdk/util/languagemanager.h>
%include <ugdk/util/intervalkdtree.h>


%template(Box2D) ugdk::ikdtree::Box<2>;
%inline %{
static ugdk::ikdtree::Box<2> CreateBox2D(double a1, double a2, double b1, double b2) {
    double min[2] = { a1, a2 };
    double max[2] = { b1, b2 };
    return ugdk::ikdtree::Box<2>(min, max);
}
%}
%template(Box3D) ugdk::ikdtree::Box<3>;

namespace ugdk {
	export_class(PathManager)
	export_class(LanguageWord)
	export_class(Language)
	export_class(LanguageManager)
	
namespace util {
    export_class(Uncopyable)
}
}
 
confirm_exports(ugdk_util)
