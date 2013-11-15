#ifndef UGDK_WINDOW_MODULE_H_
#define UGDK_WINDOW_MODULE_H_

#include <ugdk/window/manager.h>

namespace ugdk {
namespace window {

/// Initializes the module with the given Manager.
bool Initialize(Manager* manager);

/// Releases the module, disabling it.
void Release();

/// Getter for the manager of the module.
Manager* manager();

} // namespace window
} // namespace ugdk

#endif // UGDK_WINDOW_MODULE_H_
