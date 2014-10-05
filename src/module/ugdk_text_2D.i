%module ugdk_text

%include <module/util_ugdk.swig>

%include "std_map.i"
%include "std_string.i"

%{

#include <ugdk/text/font.h>
#include <ugdk/text/label.h>
#include <ugdk/text/textbox.h>
#include <ugdk/text/manager.h>
#include <ugdk/text/languageword.h>
#include <ugdk/text/language.h>

%}

%import(module="ugdk_graphic") <ugdk/graphic.h>
%import(module="ugdk_ui") <ugdk/ui.h>
%import(module="ugdk_ui") <ugdk/ui/drawable.h>

%include <ugdk/text/font.h>
%include <ugdk/text/label.h>
%include <ugdk/text/textbox.h>
%include <ugdk/text/manager.h>
%include <ugdk/text/languageword.h>
%include <ugdk/text/language.h>

namespace ugdk {
namespace text {
    export_class(Manager)
    export_class(Font)
    export_class(Label)
    export_class(TextBox)
	export_class(LanguageWord)
	export_class(Language)
}
}
 
confirm_exports(ugdk_text)
