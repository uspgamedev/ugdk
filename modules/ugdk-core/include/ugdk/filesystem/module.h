#ifndef UGDK_FILESYSTEM_MODULE_H_
#define UGDK_FILESYSTEM_MODULE_H_

#include <ugdk/filesystem/manager.h>

namespace ugdk {
namespace filesystem {

/// Initializes the module with the given Manager.
bool Initialize(Manager* manager);

/// Releases the module, disabling it.
void Release();

/// Getter for the manager of the module.
Manager* manager();

} // namespace filesystem
} // namespace ugdk

#endif // UGDK_FILESYSTEM_MODULE_H_
