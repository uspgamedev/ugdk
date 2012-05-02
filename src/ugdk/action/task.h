#ifndef UGDK_ACTION_TASK_H_
#define UGDK_ACTION_TASK_H_

#include <functional>

namespace ugdk {

namespace action {

class Task {
  public:
    Task() : finished_(false) {}
    virtual ~Task() {}

    virtual void operator()(double dt) = 0;

    bool finished() const { return finished_; }

  protected:
    bool finished_;
};

class GenericTask : public Task {
  typedef Task super;
  protected:
    std::tr1::function<bool (double)> function_;
  public:
    GenericTask(std::tr1::function<bool (double)> func) : function_(func) {}

    void operator()(double dt) {
        finished_ = function_(dt);
    }
};

} /* namespace action */

} /* namespace ugdk */

#endif /* UGDK_ACTION_TASK_H_ */
