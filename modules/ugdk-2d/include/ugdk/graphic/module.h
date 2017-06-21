#ifndef UGDK_GRAPHIC_MODULE_H_
#define UGDK_GRAPHIC_MODULE_H_

#include <ugdk/graphic/manager.h>

namespace ugdk {
namespace graphic {

/// Initializes the module with the given Manager.
bool Initialize(std::unique_ptr<Manager> manager, const std::weak_ptr<desktop::Window>&, const math::Vector2D& canvas_size);

/// Releases the module, disabling it.
void Release();

/// Getter for the manager of the module.
std::unique_ptr<Manager& > manager();

} // namespace graphic
} // namespace ugdk

#endif // UGDK_GRAPHIC_MODULE_H_
