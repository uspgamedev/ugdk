%module pyramidworks_collision

%include <module/export.swig>
%include <module/ownership.swig>
%include <module/proxy.swig>
%include "std_string.i"
%include "std_map.i"
%include "std_vector.i"
%include "std_pair.i"

%{

#include <pyramidworks/collision.h>
#include <pyramidworks/collision/collisionclass.h>
#include <pyramidworks/collision/collisionmanager.h>
#include <pyramidworks/collision/collisionobject.h>

%}

%import(module="ugdk_math") <ugdk/math/vector2D.h>
%import(module="pyramidworks_geometry") <pyramidworks/geometry.h>

%import <module/ugdk_util.i>

%newobject pyramidworks::collision::CollisionClass::FindCollidingObjects(const CollisionObject *target) const;
%newobject pyramidworks::collision::CollisionObject::absolute_position() const;
%newobject pyramidworks::collision::CollisionObject::GetBoundingBox() const;

%ignore pyramidworks::collision::CollisionManager::Generate(const char n[]);
%ignore pyramidworks::collision::CollisionManager::Generate(const char n[], const char p[]);
%ignore pyramidworks::collision::CollisionManager::Get(const char n[]);
%ignore pyramidworks::collision::CollisionManager::GenerateHandleCollisionTask();
%ignore pyramidworks::collision::CollisionObject::AddCollisionLogic(const char n[], const CollisionLogic& logic);
%ignore pyramidworks::collision::CollisionObject::InitializeCollisionClass(const char n[]);

proxy_class(pyramidworks::collision::CollisionLogic)
void_class()

%include <pyramidworks/collision.h>
%include <pyramidworks/collision/collisionclass.h>
%include <pyramidworks/collision/collisionmanager.h>

enable_disown(geometry::GeometricShape* shape)
//enable_disown(CollisionLogic* logic)
%include <pyramidworks/collision/collisionobject.h>
//disable_disown(CollisionLogic* logic)
disable_disown(geometry::GeometricShape* shape)

namespace pyramidworks {
namespace collision {
    export_class(CollisionClass)
    export_class(CollisionManager)
    export_class(CollisionObject)
}
}
 
confirm_exports(pyramidworks_collision)
