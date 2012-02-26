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

%}

//%template(DescriptionProtocol_AnimationSet) ugdk::gdd::DescriptionProtocol<AnimationSet>;

//%ignore ugdk::operator*(const double  &scalar, const Vector2D &right);
//%newobject ugdk::base::TextureContainet::Load(const std::string& filepath);

%include <ugdk/util/gdd/reader.h>
%include <ugdk/util/gdd/abstractloader.h>
%include <ugdk/util/gdd/descriptionprotocol.h>
%include <ugdk/util/gdd/parser.h>
%include <ugdk/util/gdd/simpleloader.h>
%include <ugdk/util/gdd/cachedloader.h>


namespace ugdk {
namespace gdd {
    export_class(Reader)
    export_class(LoadError)
    //export_class(ParseStatus)
}
}
 
confirm_exports(ugdk_gdd)
