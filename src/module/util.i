%module ugdk_util

%include <module/export.swig>
%include "std_map.i"
%include "std_string.i"

%{

#include <ugdk/util/animationparser.h>
#include <ugdk/util/animationprotocol.h>
#include <ugdk/util/pathmanager.h>
#include <ugdk/util/uncopyable.h>

%}

%import <module/action.i>
%import <module/gdd.i>

%template(DescriptionProtocol_AnimationSet) ugdk::gdd::DescriptionProtocol<ugdk::AnimationSet>;


//%ignore ugdk::operator*(const double  &scalar, const Vector2D &right);
//%newobject ugdk::base::TextureContainet::Load(const std::string& filepath);

%include <ugdk/util/animationparser.h>
%include <ugdk/util/animationprotocol.h>
%include <ugdk/util/pathmanager.h>
%include <ugdk/util/uncopyable.h>

namespace ugdk {
	export_class(AnimationParser)
	export_class(PathManager)
	export_class(AnimationProtocol)
	
namespace util {
    export_class(Uncopyable)
}
}
 
confirm_exports(ugdk_util)
