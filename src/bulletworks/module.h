#ifndef BULLETWORKS_MODULE_H_
#define BULLETWORKS_MODULE_H_

#include <bulletworks/manager.h>

namespace bulletworks {

/// Initializes the module with the given Manager.
bool Initialize(Manager* manager);

/// Releases the module, disabling it.
void Release();

/// Getter for the manager of the module.
Manager* manager();

} // namespace bulletworks

#endif // BULLETWORKS_MODULE_H_
