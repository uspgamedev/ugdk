#ifndef UGDK_SYSTEM_ENGINE_H_
#define UGDK_SYSTEM_ENGINE_H_

#include <vector>
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
#include <ugdk/math/vector2D.h>

namespace ugdk {

class PathManager;

/// The game engine. Manages scenes, time, and the Audio, Video, Input and Text managers.
/** Engine is a singleton. Use Engine::reference to get a pointer to an engine.
 * @see reference()
 */
class Engine {
  public:

    /// Returns a pointer to the current Engine. Creates an Engine if there isn't one.
    static Engine* reference() { return reference_ ? reference_ : reference_ = new Engine; }

    /// Returns a reference to the Video Manager.
    /** @see VideoManager
     */
    graphic::VideoManager *video_manager() { return video_manager_; }

    /// Returns a reference to the Text Manager.
    /** @see TextManager
     */
    graphic::TextManager *text_manager() { return text_manager_; }

    /// Returns a reference to the Time Handler.
    /** @see TimeManager
     */
    time::TimeManager *time_handler() { return time_manager_; }
    /// Returns a reference to the Path Manager.
    /** @see PathManager
     */
    PathManager *path_manager() { return path_manager_; }
    /// Returns a reference to the Resource Manager.
    /** @see Manager
     */
    resource::Manager *resource_manager() { return resource_manager_; }

    /// Returns a reference to the Language Manager.
    /** @see LanguageManager
     */
    LanguageManager* language_manager() { return language_manager_; }

    /// Returns the window dimensions.
    ugdk::math::Vector2D window_size();

    /** @name Engine Management
     ** @{
     */

    /// Initializes the engine. Returns true if sucessful, false otherwise.
    /** @param configuration A Engine::Configuration struct with the planned
     **                      configuration.
     */
	bool Initialize(const Configuration& configuration);

    /// Initializes the engine. Calls the other Initialize method with all default arguments.
    bool Initialize() {
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
    action::Scene* CurrentScene() const;
    /// Removes the top scene.
    void PopScene();
    /** @}
     */

    /// Returns the current running FPS.
    uint32 current_fps() const { return reported_fps_; }

    // Saida do motor
    /// Stops the engine and clears the scene list.
    void quit() { quit_ = true; }

    ~Engine() { reference_ = nullptr; }

  private:
    void DeleteFinishedScenes();
    static Engine         *    reference_;

    graphic::VideoManager *       video_manager_;
    graphic:: TextManager *        text_manager_;
    time::    TimeManager *        time_manager_;
	          PathManager *        path_manager_;
    resource::Manager *resource_manager_;
          LanguageManager *    language_manager_;

    bool quit_;
    std::list<action::Scene*> scene_list_;
    uint32 reported_fps_, frames_since_reset_, last_fps_report_;

	Engine() :
        video_manager_(nullptr),
        text_manager_(nullptr),
        time_manager_(nullptr),
        resource_manager_(nullptr), 
        language_manager_(nullptr) {}
};

} // namespace ugdk

#endif

