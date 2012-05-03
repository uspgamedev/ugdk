#ifndef UGDK_ACTION_TASK_H_
#define UGDK_ACTION_TASK_H_

#include <functional>

namespace ugdk {

namespace action {

class Task {
  public:
    Task() : finished_(false), priority_(0) {}
    Task(int priority) : finished_(false), priority_(priority) {}
    virtual ~Task() {}

    virtual void operator()(double dt) = 0;
    int priority() const { return priority_; }

    bool finished() const { return finished_; }

  protected:
    bool finished_;
    int priority_;
};

template <class Data>
class GenericTask : public Task {
  typedef Task super;
  protected:
    Data data_;
    std::tr1::function<bool (double, Data&)> function_;
  public:
    GenericTask(std::tr1::function<bool (double, Data&)> func, const Data& data) 
        : function_(func), data_(data) {}

    GenericTask(std::tr1::function<bool (double, Data&)> func, const Data& data, int priority) 
        : Task(priority), function_(func), data_(data) {}

    void operator()(double dt) {
        finished_ = !function_(dt, data_);
    }
};

} /* namespace action */

} /* namespace ugdk */

#endif /* UGDK_ACTION_TASK_H_ */
