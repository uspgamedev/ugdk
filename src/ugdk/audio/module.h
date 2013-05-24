#ifndef UGDK_AUDIO_MODULE_H_
#define UGDK_AUDIO_MODULE_H_

#include <ugdk/audio/manager.h>

namespace ugdk {
namespace audio {

/// Initializes the module with the given Manager.
bool Initialize(Manager* manager);

/// Releases the module, disabling it.
void Release();

/// Getter for the manager of the module.
Manager* manager();

} // namespace audio
} // namespace ugdk

#endif // UGDK_AUDIO_MODULE_H_
