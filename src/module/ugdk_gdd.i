%module ugdk_gdd

%include <module/export.swig>
%include "std_map.i"
%include "std_string.i"
%include "std_vector.i"

%{

#include <ugdk/util/gdd/reader.h>
#include <ugdk/util/gdd/abstractloader.h>
#include <ugdk/util/gdd/descriptionprotocol.h>
#include <ugdk/util/gdd/parser.h>
#include <ugdk/util/gdd/simpleloader.h>
#include <ugdk/util/gdd/cachedloader.h>

#include <ugdk/action/animationset.h>

%}

%import(module="ugdk_action") <ugdk/action/animationset.h>

%include <ugdk/util/gdd/reader.h>
%include <ugdk/util/gdd/abstractloader.h>
%include <ugdk/util/gdd/descriptionprotocol.h>
%include <ugdk/util/gdd/parser.th>
%include <ugdk/util/gdd/simpleloader.th>
%include <ugdk/util/gdd/cachedloader.th>

%template(AnimationAbstractLoader) ugdk::gdd::AbstractLoader<ugdk::action::AnimationSet>;
%template(AnimationSimpleLoader) ugdk::gdd::SimpleLoader<ugdk::action::AnimationSet>;
%template(AnimationLoader) ugdk::gdd::CachedLoader<ugdk::action::AnimationSet>;


namespace ugdk {
namespace gdd {
    export_class(Reader)
    export_class(LoadError)
    //export_class(ParseStatus)
}
}
 
confirm_exports(ugdk_gdd)
