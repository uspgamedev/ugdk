#include <ugdk/system/taskplayer.h>

#include <algorithm>

namespace ugdk {
namespace system {

using std::list;

void TaskPlayer::AddTask(const Task& task) {
    tasks_.merge(list<Task>(1, task));
}

void TaskPlayer::Update(double dt) {
    // For each task, do. Note the tasks list is already ordered by the priorities.
    tasks_.remove_if([dt](Task& task) {
        // Calls the task, and removes it from the list if the task returned false.
        return !task(dt);
    });
}

} // namespace system
} // namespace ugdk
