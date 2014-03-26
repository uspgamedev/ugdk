#ifndef UGDK_GRAPHIC_PRIMITIVE_CONTROLLER_H_
#define UGDK_GRAPHIC_PRIMITIVE_CONTROLLER_H_

#include <ugdk/math.h>
#include <ugdk/graphic.h>

namespace ugdk {
namespace graphic {

class PrimitiveController {
  public:         
    virtual ~PrimitiveController() {}

    virtual void set_owner(Primitive* owner) = 0;
};

class PrimitiveControllerPosition : public virtual PrimitiveController {
  public:
    virtual ~PrimitiveControllerPosition() {}

    virtual void ChangePosition(const math::Vector2D& position) = 0;
};

}  // namespace graphic
}  // namespace ugdk

#endif // UGDK_GRAPHIC_PRIMITIVE_CONTROLLER_H_
