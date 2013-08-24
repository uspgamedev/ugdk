#ifndef UGDK_SYSTEM_TASKPLAYER_H_
#define UGDK_SYSTEM_TASKPLAYER_H_

#include <list>
#include <ugdk/system.h>

namespace ugdk {
namespace system {

class TaskPlayer {
  private:

    /// Auxiliar class for AddTask.
    template <typename Ret>
    struct TaskAdapter {
        template <typename T>
        static Task Adapt(T t) {
            return Task(t);
        }
    };
    // struct TaskPlayer::TaskAdapter<void>
    // Located after TaskPlayer definition.

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
    template <typename T>
    void AddTask(T task, double priority = 0.5) {
        AddFunctionTask(TaskAdapter<decltype(task(0.0))>::Adapt(task), priority);
    }

    /// Logical update of the scene.
    /**
       @param delta_t Time in seconds since last update
    */
    void Update(double delta_t);

  private:
    void AddFunctionTask(const Task& task, double priority);

    struct OrderedTask {
        OrderedTask(double p, const Task& t) : priority(p), task(t) {}

        double priority;
        Task task;

        bool operator< (const OrderedTask& other) const { 
            return priority < other.priority;
        }
    };
    std::list<OrderedTask> tasks_;
};

template <>
struct TaskPlayer::TaskAdapter<void> {
    template <typename T>
    static Task Adapt(T t) {
        return Task([t](double dt) -> bool {
            t(dt);
            return true;
        });
    }
};

} // namespace action */
} // namespace ugdk */

#endif /* UGDK_SYSTEM_TASKPLAYER_H_ */
