#ifndef UGDK_RESOURCE_MODULE_H_
#define UGDK_RESOURCE_MODULE_H_

#include <ugdk/resource/manager.h>

#include <ugdk/system/config.h>

#include <ugdk/action.h>
#include <ugdk/graphic.h>
#include <ugdk/text.h>
#include <ugdk/structure.h>
#include <ugdk/action/json2spriteanimation.h>
#include <memory>

namespace ugdk {
namespace resource {

bool is_active();


/// Initializes the module with the given Manager.
bool Initialize(std::unique_ptr<Manager> manager);

/// Releases the module, disabling it.
void Release();

/// Getter for the manager of the module.
resource::Manager& manager();


} // namespace resource
} // namespace ugdk

#endif // UGDK_RESOURCE_MODULE_H_
