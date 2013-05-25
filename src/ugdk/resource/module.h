#ifndef UGDK_RESOURCE_MODULE_H_
#define UGDK_RESOURCE_MODULE_H_

#include <ugdk/resource/manager.h>

namespace ugdk {
namespace resource {

/// Initializes the module with the given Manager.
bool Initialize(Manager* manager);

/// Releases the module, disabling it.
void Release();

/// Getter for the manager of the module.
Manager* manager();

} // namespace resource
} // namespace ugdk

#endif // UGDK_RESOURCE_MODULE_H_

