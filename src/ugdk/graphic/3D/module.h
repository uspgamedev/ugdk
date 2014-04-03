#ifndef UGDK_GRAPHIC_3D_MODULE_H_
#define UGDK_GRAPHIC_3D_MODULE_H_

#include <ugdk/graphic/3D/manager.h>

namespace ugdk {
namespace graphic {
namespace threed {

/// Initializes the module with the given Manager.
bool Initialize(Manager* manager);

/// Releases the module, disabling it.
void Release();

/// Getter for the manager of the module.
Manager* manager();

} // namespace threed
} // namespace graphic
} // namespace ugdk

#endif // UGDK_GRAPHIC_3D_MODULE_H_
