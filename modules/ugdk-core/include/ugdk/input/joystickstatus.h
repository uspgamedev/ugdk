#ifndef UGDK_INPUT_JOYSTICKSTATUS_H_
#define UGDK_INPUT_JOYSTICKSTATUS_H_

#include <ugdk/structure/types.h>

namespace ugdk {
namespace input {

class AxisStatus {
  public: 
    AxisStatus(int16 raw);

    int16 raw() const;
    int16 MaximumValue() const;
    int16 MinimumValue() const;
    double Percentage() const;

  private:
    int16 raw_;
};

class HatStatus {
  public:
    HatStatus(int raw);

    int raw() const;
    bool IsCentered() const;
    bool IsUp() const;
    bool IsRight() const;
    bool IsDown() const;
    bool IsLeft() const;
    bool IsRightUp() const;
    bool IsRightDown() const;
    bool IsLeftUp() const;
    bool IsLeftDown() const;

    bool HasUp() const;
    bool HasRight() const;
    bool HasDown() const;
    bool HasLeft() const;

  private:
    int raw_;
};

} // namespace input
} // namespace ugdk

#endif // UGDK_INPUT_JOYSTICKSTATUS_H_
