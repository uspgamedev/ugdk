#ifndef HORUSEYE_FRAMEWORK_OBSERVER_H_
#define HORUSEYE_FRAMEWORK_OBSERVER_H_
namespace ugdk{
class Observer{
  public:
    virtual void Tick() = 0;
};
}

#endif
