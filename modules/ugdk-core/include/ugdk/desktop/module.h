#ifndef UGDK_DESKTOP_MODULE_H_
#define UGDK_DESKTOP_MODULE_H_

#include <ugdk/desktop/manager.h>

#include <memory>

namespace ugdk {
namespace desktop {

/// Initializes the module with the given Manager.
bool Initialize(std::unique_ptr<Manager> manager);
bool is_active();
/// Releases the module, disabling it.
void Release();

/// Getter for the manager of the module.
desktop::Manager& manager();

} // namespace desktop
} // namespace ugdk

#endif // UGDK_DESKTOP_MODULE_H_
