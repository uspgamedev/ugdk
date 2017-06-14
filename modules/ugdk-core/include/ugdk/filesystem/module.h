#ifndef UGDK_FILESYSTEM_MODULE_H_
#define UGDK_FILESYSTEM_MODULE_H_

#include <ugdk/filesystem/manager.h>
#include <memory>

namespace ugdk {
namespace filesystem {

/// Initializes the module with the given Manager.
bool Initialize(std::unique_ptr<Manager> manager);

/// Releases the module, disabling it.
void Release();

/// Getter for the manager of the module.
filesystem::Manager& manager();

} // namespace filesystem
} // namespace ugdk

#endif // UGDK_FILESYSTEM_MODULE_H_
