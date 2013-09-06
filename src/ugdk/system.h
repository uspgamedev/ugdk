#ifndef UGDK_SYSTEM_H_
#define UGDK_SYSTEM_H_

#include <functional>

namespace ugdk {
namespace system {

struct Configuration;
class EventHandler;
class TaskPlayer;

typedef std::function<bool (double)> Task;

}  // namespace system
}  // namespace ugdk

#endif
