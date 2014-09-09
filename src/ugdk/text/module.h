#ifndef UGDK_TEXT_MODULE_H_
#define UGDK_TEXT_MODULE_H_

#include <ugdk/text/manager.h>

namespace ugdk {
namespace text {

/// Initializes the module with the given Manager.
bool Initialize(Manager* manager);

/// Releases the module, disabling it.
void Release();

/// Getter for the manager of the module.
Manager* manager();

} // namespace text
} // namespace ugdk

#endif // UGDK_TEXT_MODULE_H_
