#include <ugdk/system/taskplayer.h>

#include <algorithm>

namespace ugdk {
namespace system {

using std::list;

void TaskPlayer::Update(double dt) {
    // For each task, do. Note the tasks list is already ordered by the priorities.
    tasks_.remove_if([dt](OrderedTask& otask) {
        // Calls the task, and removes it from the list if the task returned false.
        return !otask.task(dt);
    });
}

void TaskPlayer::AddFunctionTask(const Task& task, double priority) {
    tasks_.merge(list<OrderedTask>(1, OrderedTask(priority, task)));
}

} // namespace system
} // namespace ugdk
