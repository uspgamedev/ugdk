#ifndef UGDK_SYSTEM_ENGINE_H_
#define UGDK_SYSTEM_ENGINE_H_

#include <ugdk/action.h>
#include <ugdk/debug.h>
#include <ugdk/internal.h>
#include <ugdk/system.h>
#include <ugdk/util.h>
#include <ugdk/structure/types.h>
#include <ugdk/system/configuration.h>
#include <ugdk/system/compatibility.h>

#include <functional>
#include <list>
#include <memory>
#include <string>

#ifdef SWIG
#define GET_OWNERSHIP(T) T&&
#else
#define GET_OWNERSHIP(T) T
#endif

namespace ugdk {
namespace system {

typedef std::function<std::unique_ptr<action::Scene> ()> SceneFactory;

/// Expands the given path with configurated base path.
std::string ResolvePath(const std::string& path);
std::string GetFileContents(const std::string& filename);

/** @name Engine Management
 ** @{
 */

/// Initializes the engine. Returns true if sucessful, false otherwise.
/** @param configuration A Engine::Configuration struct with the planned
 **                      configuration.
 */
bool Initialize(const Configuration& configuration);

/// Initializes the engine. Calls the other Initialize method with all default arguments.
bool Initialize();

/// Starts running the engine.
void Run();

/// Releases all the resouces allocated by the engine.
void Release();

/** @}
 */

/** @name Scene Management
 * @{
 */

/// Queues a scene to be added just before the start of the next frame.
/** @param scene_factory A factory for the scene to be added. */
void PushSceneFactory(const std::function<std::unique_ptr<action::Scene>()>& scene_factory);

/// Queues a scene to be added just before the start of the next frame.
/** @param scene The scene to be added. */
void PushScene(GET_OWNERSHIP(std::unique_ptr<action::Scene>) scene);

/// Returns the currently focused scene.
action::Scene& CurrentScene();

/// Returns the scene list.
/** The list is changed when PushScene is called. */
const std::list<std::unique_ptr<action::Scene>>& scene_list();


const std::list<std::shared_ptr<const debug::SectionData>>& profile_data_list();

/** @}
 */

/// Stops the main loop.
void Suspend();


/** @name Internal
 * @{
 */

void RegisterSDLHandler(const internal::SDLEventHandler*);
void DeregisterSDLHandler(const internal::SDLEventHandler*);

/** @}
 */

} // namespace system
} // namespace ugdk

#endif

