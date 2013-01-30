#ifndef UGDK_ACTION_TASK_H_
#define UGDK_ACTION_TASK_H_

#include <ugdk/util/uncopyable.h>

namespace ugdk {
namespace action {

class Task : public util::Uncopyable {
  public:
    Task() : finished_(false), priority_(0) {}
    Task(int _priority) : finished_(false), priority_(_priority) {}
    virtual ~Task() {}

    virtual void operator()(double dt) = 0;

    int priority() const { return priority_; }

    bool finished() const { return finished_; }

  protected:
    bool finished_;

  private:
    const int priority_;
};

} /* namespace action */

} /* namespace ugdk */

#endif /* UGDK_ACTION_TASK_H_ */
