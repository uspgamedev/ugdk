#ifndef UGDK_EVENT_MODULE_H_
#define UGDK_EVENT_MODULE_H_

#include <ugdk/event/manager.h>

namespace ugdk {
namespace event {

/// Initializes the module with the given Manager.
bool Initialize(Manager* manager);

/// Releases the module, disabling it.
void Release();

/// Getter for the manager of the module.
Manager* manager();

} // namespace event
} // namespace ugdk

#endif // UGDK_EVENT_MODULE_H_
