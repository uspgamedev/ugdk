#ifndef UGDK_ACTION_GENERICTASK_H_
#define UGDK_ACTION_GENERICTASK_H_

#include <functional>
#include <ugdk/action/task.h>

namespace ugdk {

namespace action {

class GenericTask : public Task {
  typedef Task super;
  protected:
    std::tr1::function<bool (double)> function_;
  public:
    GenericTask(std::tr1::function<bool (double)> func)  : function_(func) {}

    GenericTask(std::tr1::function<bool (double)> func, int priority) 
        : Task(priority), function_(func) {}

    void operator()(double dt) {
        finished_ = !function_(dt);
    }
};

} /* namespace action */

} /* namespace ugdk */

#endif /* UGDK_ACTION_GENERICTASK_H_ */
