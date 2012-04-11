#ifndef HORUSEYE_FRAMEWORK_OBSERVER_H_
#define HORUSEYE_FRAMEWORK_OBSERVER_H_
namespace ugdk{

namespace action{
class Observer{
  public:
    virtual void Tick() = 0;
};

} /* namespace action */

} /* namespace ugdk */

#endif /* HORUSEYE_FRAMEWORK_OBSERVER_H_ */
