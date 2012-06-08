#ifndef PYRAMIDWORKS_COLLISION_GENERICCOLLISIONLOGIC_H_
#define PYRAMIDWORKS_COLLISION_GENERICCOLLISIONLOGIC_H_

#include <functional>
#include <pyramidworks/collision/collisionlogic.h>

namespace pyramidworks {
namespace collision {

/// \class GenericCollisionLogic genericcollisionlogic.h "pyramidworks/collision/genericcollisionlogic.h"
/// CollisionLogic implementation using std::tr1::function.
class GenericCollisionLogic : public CollisionLogic {
  public:
    GenericCollisionLogic(std::tr1::function<void (void*)> func)
        : func_(func) {}
    ~GenericCollisionLogic() {}

    /// Method that is called when a collision happens.
    /** @param data The data attribute of an CollisionObject. */
    void Handle(void *data) {
        func_(data);
    }

  private:
    std::tr1::function<void (void*)> func_;
};

} // namespace collision
} // namespace pyramidworks

#endif /* PYRAMIDWORKS_COLLISION_GENERICCOLLISIONLOGIC_H_ */
