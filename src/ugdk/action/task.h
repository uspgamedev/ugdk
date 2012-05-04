#ifndef UGDK_ACTION_TASK_H_
#define UGDK_ACTION_TASK_H_

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

} /* namespace action */

} /* namespace ugdk */

#endif /* UGDK_ACTION_TASK_H_ */
