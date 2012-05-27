#ifndef UGDK_ACTION_OBSERVERFUNCTION_H_
#define UGDK_ACTION_OBSERVERFUNCTION_H_

#include <ugdk/action/observer.h>
#include <functional>

namespace ugdk {

namespace action {

class ObserverFunction : public Observer {
  public:
    ObserverFunction(std::tr1::function<void (void)> tick) : tick_(tick) {}
    ~ObserverFunction() {}

    void Tick() { tick_(); }

  private:
    std::tr1::function<void (void)> tick_;
};

} /* namespace action */

} /* namespace ugdk */

#endif /* UGDK_ACTION_OBSERVERFUNCTION_H_ */
