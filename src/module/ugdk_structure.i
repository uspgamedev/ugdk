%module ugdk_structure

%include <module/util_ugdk.swig>

%include "std_string.i"

%{

#include <ugdk/structure.h>
#include <ugdk/structure/types.h>
#include <ugdk/structure/box.h>
#include <ugdk/structure/indexabletable.h>
#include <ugdk/structure/intervalkdtree.h>
#include <ugdk/structure/color.h>
#include <ugdk/structure/visualeffect.h>

%}

%include <ugdk/structure.h>
%include <ugdk/structure/types.h>
%include <ugdk/structure/box.h>
%include <ugdk/structure/indexabletable.h>
%include <ugdk/structure/intervalkdtree.h>
%include <ugdk/structure/color.h>
%include <ugdk/structure/visualeffect.h>


%template(Box2D) ugdk::structure::Box<2>;
%inline %{
static ugdk::structure::Box<2> CreateBox2D(double a1, double a2, double b1, double b2) {
    std::array<double, 2> min = {{ a1, a2 }}, max = {{ b1, b2 }};
    return ugdk::structure::Box<2>(min, max);
}
%}
%template(Box3D) ugdk::structure::Box<3>;

namespace ugdk {
namespace structure {
    export_class(Color)
}
}
 
confirm_exports(ugdk_structure)
