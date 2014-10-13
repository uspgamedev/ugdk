#ifndef UGDK_ACTION_OBSERVER_H_
#define UGDK_ACTION_OBSERVER_H_

namespace ugdk {

namespace action {

class Observer {
  public:
    virtual ~Observer() {}
    virtual void Tick() = 0;
};

} /* namespace action */

} /* namespace ugdk */

#endif /* UGDK_ACTION_OBSERVER_H_ */
