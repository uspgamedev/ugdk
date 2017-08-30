#ifndef UGDK_GRAPHIC_PRIMITIVE_CONTROLLER_H_
#define UGDK_GRAPHIC_PRIMITIVE_CONTROLLER_H_

#include <ugdk/math/vector2D.h>

#include <ugdk/math.h>
#include <ugdk/graphic.h>

namespace ugdk {
namespace graphic {

class PrimitiveController {
  public:       
    PrimitiveController() : owner_(nullptr) {}
    virtual ~PrimitiveController() {}

    virtual void ChangePosition(const math::Vector2D& position);
    virtual void set_owner(Primitive* owner) {
        owner_ = owner;
    }

  protected:
    Primitive* owner_;
    math::Vector2D position_;
};

}  // namespace graphic
}  // namespace ugdk

#endif // UGDK_GRAPHIC_PRIMITIVE_CONTROLLER_H_
