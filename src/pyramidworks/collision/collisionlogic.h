#ifndef PYRAMIDWORKS_COLLISION_COLLISIONLOGIC_H_
#define PYRAMIDWORKS_COLLISION_COLLISIONLOGIC_H_

namespace pyramidworks {
namespace collision {

#define COLLISION_DIRECT(DATA, NAME, ARG) class NAME : public pyramidworks::collision::CollisionLogic { \
	protected:	DATA data_; \
	public:		NAME ( DATA data) : data_(data) {} \
	void Handle(void*); }; \
    void NAME::Handle(void* ARG)

/// \class CollisionLogic collisionlogic.h "pyramidworks/collision/collisionlogic.h"
/// Class that handles what happens when a collision happens.
/** Use the macro COLLISION_DIRECT to create a new class that implements the 
  * Handle method and contains a data variable. */
class CollisionLogic {
  public:
    virtual ~CollisionLogic() {}

    /// Method that is called when a collision happens.
    /** @param data The data attribute of an CollisionObject. */
    virtual void Handle(void *data) = 0;

  protected:
    CollisionLogic() {}
};

} // namespace collision
} // namespace pyramidworks

#endif /* PYRAMIDWORKS_COLLISION_COLLISIONLOGIC_H_ */
