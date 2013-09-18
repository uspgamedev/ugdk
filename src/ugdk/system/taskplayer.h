#ifndef UGDK_SYSTEM_TASKPLAYER_H_
#define UGDK_SYSTEM_TASKPLAYER_H_

#include <list>
#include <ugdk/system.h>
#include <ugdk/system/task.h>

namespace ugdk {
namespace system {

class TaskPlayer {
  public:
    TaskPlayer() {}
    virtual ~TaskPlayer() {}

    /// Adds a task to the scene.
    /** All tasks are called for each Scene's Update, in order of the priorities, which
        are ordered from smaller values to bigger values.

        The task's returned value is used to destroy the task or keep it alive:
        - True keeps then for another Update.
        - False destroys then.
        - void return type implies in 'always true'.
    
        Priority values are commonly in the [0; 1] interval. */
    void AddTask(const Task& task);

    /// Logical update of the scene.
    /**
       @param delta_t Time in seconds since last update
    */
    void Update(double delta_t);

  private:
    std::list<Task> tasks_;
};

} // namespace action */
} // namespace ugdk */

#endif /* UGDK_SYSTEM_TASKPLAYER_H_ */
