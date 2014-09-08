%module pyramidworks_collision

%include <module/util_ugdk.swig>

%include "std_string.i"
%include "std_map.i"
%include "std_vector.i"
%include "std_pair.i"

%{

#include <pyramidworks/collision.h>
#include <pyramidworks/collision/collisiondata.h>
#include <pyramidworks/collision/collisionclass.h>
#include <pyramidworks/collision/collisionmanager.h>
#include <pyramidworks/collision/collisionobject.h>

%}

%import(module="ugdk_math") <ugdk/math/vector2D.h>
%import(module="pyramidworks_geometry") <pyramidworks/geometry.h>

%import <module/ugdk_util.i>

%newobject pyramidworks::collision::CollisionClass::FindCollidingObjects(const CollisionObject *target) const;
%newobject pyramidworks::collision::CollisionObject::absolute_position() const;
%newobject pyramidworks::collision::CollisionObject::CreateBoundingBox() const;

%ignore pyramidworks::collision::CollisionManager::GenerateHandleCollisionTask(double priority);

proxy_class(pyramidworks::collision::CollisionLogic)
void_class()

enable_disown(pyramidworks::geometry::GeometricShape* shape)
%include <pyramidworks/collision.h>
%include <pyramidworks/collision/collisiondata.h>
%include <pyramidworks/collision/collisionclass.h>
%include <pyramidworks/collision/collisionmanager.h>
%include <pyramidworks/collision/collisionobject.h>
disable_disown(pyramidworks::geometry::GeometricShape* shape)

namespace pyramidworks {
namespace collision {
	export_class(CollisionData)
    export_class(CollisionClass)
    export_class(CollisionManager)
    export_class(CollisionObject)
}
}
 
confirm_exports(pyramidworks_collision)
