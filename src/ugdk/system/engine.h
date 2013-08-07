#ifndef UGDK_SYSTEM_ENGINE_H_
#define UGDK_SYSTEM_ENGINE_H_

#include <list>
#include <string>
#include <ugdk/action.h>
#include <ugdk/audio.h>
#include <ugdk/graphic.h>
#include <ugdk/input.h>
#include <ugdk/resource.h>
#include <ugdk/time.h>
#include <ugdk/util.h>
#include <ugdk/common/types.h>
#include <ugdk/system/configuration.h>

namespace ugdk {
namespace system {

class PathManager;

/// Returns a reference to the Text Manager.
/** @see TextManager
 */
graphic::TextManager *text_manager();

/// Returns a reference to the Language Manager.
/** @see LanguageManager
 */
LanguageManager* language_manager();

/// Expands the given path with configurated base path.
std::string ResolvePath(const std::string& path);

/** @name Engine Management
 ** @{
 */

/// Initializes the engine. Returns true if sucessful, false otherwise.
/** @param configuration A Engine::Configuration struct with the planned
 **                      configuration.
 */
bool Initialize(const Configuration& configuration);

/// Initializes the engine. Calls the other Initialize method with all default arguments.
inline bool Initialize() {
    Configuration defaults;
    return Initialize(defaults);
}

/// Starts running the engine.
void Run();

/// Releases all the resouces allocated by the engine.
void Release();

/** @}
 */

/** @name Scene Management
 * @{
 */
/// Puts a scene onto the scene list, on top.
/** @param scene The scene to be put.
 */
void PushScene(action::Scene* scene);

/// Returns the current scene.
action::Scene* CurrentScene();

/// Returns the scene list.
/** The list is changed when PushScene is called. */
const std::list<action::Scene*>& scene_list();

/// Removes the top scene.
void PopScene();
/** @}
 */

/// Stops the engine and clears the scene list.
void Quit();

} // namespace system
} // namespace ugdk

#endif

