#ifndef UGDK_TIME_MODULE_H_
#define UGDK_TIME_MODULE_H_

#include <ugdk/time/manager.h>
#include <memory>

namespace ugdk {
namespace time {

/// Initializes the module with the given Manager.
bool Initialize(std::unique_ptr<Manager> manager);
bool is_active();
/// Releases the module, disabling it.
void Release();

/// Getter for the manager of the module.
time::Manager& manager();

} // namespace time
} // namespace ugdk

#endif // UGDK_TIME_MODULE_H_
