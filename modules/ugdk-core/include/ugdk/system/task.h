#ifndef UGDK_SYSTEM_TASK_H_
#define UGDK_SYSTEM_TASK_H_

#include <functional>

namespace ugdk {
namespace system {

class Task {
public:
    /** Constructs a new Task, with given callable and priority.
        Priority is tipically in the range [0, 1], and smaller priorities are executed before. 
        @see TaskPlayer */
    template<typename Callable>
    Task(Callable c, double priotity = 0.5) : priority_(priotity) {
        CallableAdapter<decltype(c(0.0))>::Set(function_, c);
    }
    ~Task() {}

    double priority() const { return priority_; }
    bool operator()(double dt) {
        return function_(dt);
    }
    bool operator<(const Task& other) const {
        return priority_ < other.priority_;
    }

private:
    template<typename Ret>
    struct CallableAdapter {
        template<typename Callable>
        static void Set(std::function<bool (double)>& function, Callable c) {
            function = c;
        }
    };

    double priority_;
    std::function<bool (double)> function_;
};

template<>
struct Task::CallableAdapter<void> {
    template<typename Callable>
    static void Set(std::function<bool (double)>& function, Callable c) {
        function = [c](double dt) {
            c(dt);
            return true;
        };
    }
};

} // namespace action */
} // namespace ugdk */

#endif /* UGDK_SYSTEM_TASKPLAYER_H_ */
