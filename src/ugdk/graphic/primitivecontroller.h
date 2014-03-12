#ifndef UGDK_GRAPHIC_PRIMITIVE_CONTROLLER_H_
#define UGDK_GRAPHIC_PRIMITIVE_CONTROLLER_H_

namespace ugdk {
namespace graphic {

class PrimitiveControllerPosition {
  public:
    virtual ~PrimitiveControllerPosition() {}
    virtual void ChangePosition(const math::Vector2D& position) = 0;
};

}  // namespace graphic
}  // namespace ugdk

#endif // UGDK_GRAPHIC_PRIMITIVE_CONTROLLER_H_
