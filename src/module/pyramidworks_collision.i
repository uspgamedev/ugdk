%module pyramidworks_collision

%include <module/export.swig>
%include "std_string.i"
%include "std_map.i"
%include "std_list.i"

%{

#include <pyramidworks/collision/collisionlogic.h>
#include <pyramidworks/collision/collisionclass.h>
#include <pyramidworks/collision/collisionmanager.h>
#include <pyramidworks/collision/collisionobject.h>

%}

%import(module="ugdk_math") <ugdk/math/vector2D.h>
%import(module="pyramidworks_geometry") <pyramidworks/geometry.h>

%import <module/util.i>

%include <pyramidworks/collision/collisionlogic.h>
%include <pyramidworks/collision/collisionclass.h>
%include <pyramidworks/collision/collisionmanager.h>
%include <pyramidworks/collision/collisionobject.h>

namespace pyramidworks {
namespace collision {
    export_class(CollisionLogic)
    export_class(CollisionClass)
    export_class(CollisionManager)
    export_class(CollisionObject)
}
}
 
confirm_exports(pyramidworks_collision)
