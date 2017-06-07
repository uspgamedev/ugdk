#ifndef UGDK_RESOURCE_MODULE_H_
#define UGDK_RESOURCE_MODULE_H_

#include <ugdk/resource/manager.h>

#include <ugdk/system/config.h>

#include <ugdk/action.h>
#include <ugdk/graphic.h>
#include <ugdk/text.h>
#include <ugdk/structure.h>
#include <ugdk/action/json2spriteanimation.h>

namespace ugdk {
namespace resource {

/// Initializes the module with the given Manager.
bool Initialize(Manager* manager);

/// Releases the module, disabling it.
void Release();

/// Getter for the manager of the module.
Manager* manager();

std::shared_ptr<action::SpriteAnimationTable>    GetSpriteAnimationTableFromFile(const std::string& file);

} // namespace resource
} // namespace ugdk

#endif // UGDK_RESOURCE_MODULE_H_

