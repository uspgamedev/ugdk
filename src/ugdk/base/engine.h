#ifndef UGDK_BASE_ENGINE_H_
#define UGDK_BASE_ENGINE_H_

#include <vector>
#include <list>
#include <string>
#include <ugdk/math/vector2D.h>
#include <ugdk/action/scene.h>
#include <ugdk/action/layer.h>
#include <ugdk/util/gdd/cachedloader.h>
#include <ugdk/util/animationprotocol.h>

namespace ugdk {

class VideoManager;
class InputManager;
class TimeHandler;
class AudioManager;
class TextManager;
class PathManager;
class AnimationSet;
class Scene;

typedef gdd::CachedLoader<AnimationSet> AnimationLoader;

/// The game engine. Manages scenes, time, and the Audio, Video, Input and Text managers.
/** Engine is a singleton. Use Engine::reference to get a pointer to an engine.
 * @see reference()
 */
class Engine {
  public:
    // Engine e' um singleton. Use Engine::reference()
    // para obter um ponteiro para o objeto desta classe.
    /// Returns a pointer to the current Engine. Creates an Engine if there isn't one.
    static Engine* reference() { static Engine *r = NULL;
                                 return r ? r : r = new Engine; }

    /// Returns a reference to the Video Manager.
    /** @see VideoManager
     */
    VideoManager *video_manager() { return video_manager_; }
    /// Returns a reference to the Input Manager.
    /** @see InputManager
     */
    InputManager *input_manager() { return input_manager_; }
    /// Returns a reference to the Time Handler.
    /** @see TimeHandler
     */
    TimeHandler *time_handler() { return time_handler_; }
    /// Returns a reference to the Audio Manager.
    /** @see AudioManager
     */
    AudioManager *audio_manager() { return audio_manager_; }
    /// Returns a reference to the Text Manager.
    /** @see TextManager
     */
    TextManager *text_manager() { return text_manager_; }
    /// Returns a reference to the Path Manager.
    /** @see PathManager
     */
    PathManager *path_manager() { return path_manager_; }

    /// Returns a reference to the Animation Loader.
    /** @see AnimationLoader
     */
    AnimationLoader& animation_loader() { return animation_loader_; }

    /// Returns the window dimensions.
    Vector2D window_size();

    // Gerenciamento do motor
    /**@name Engine Management
     * @{
     */
    /// Initializes the engine. Returns true if sucessful, false otherwise.
    /** @param windowTitle The window title.
     * @param windowSize The window's dimensions.
     */
	bool Initialize(std::string windowTitle, Vector2D windowSize, 
		bool fullscreen,
		std::string base_path = std::string("./"),
		std::string icon = std::string());
    /// Starts running the engine.
    void Run();
    /// Stops and frees the engine.
    void Release();

    /** @}
     */

    /** @name Scene Management
     * @{
     */
    // Gerenciamento de cenas

    /// Puts a scene onto the scene list, on top.
    /** @param scene The scene to be put.
     */
    void PushScene(Scene* scene);
    /// Returns the current scene.
    Scene* CurrentScene() const;
    /// Removes the top scene.
    void PopScene();

    // TODO: SERIOUSLY FIX THIS. Big fat ugly code to allow some special 
    // layers to be unnafected by the light system. Meant for user interface.
    /// Pushes a layer unaffected by the light system. Meant for interfaces.
    void PushInterface(Layer* layer);
    /// Removes a layer pushed by the above function.
    void RemoveInterface(Layer *layer);
    /** @}
     */
    /// Returns the current running FPS.
    uint32 current_fps() { return reported_fps_; }

    // Saida do motor
    /// Stops the engine and clears the scene list.
    void quit() { quit_ = true; }

    // outros
    ~Engine() {}

  private:
    void DeleteFinishedScenes();
    VideoManager *video_manager_;
    InputManager *input_manager_;
    TimeHandler *time_handler_;
    AudioManager *audio_manager_;
    TextManager *text_manager_;
	PathManager *path_manager_;
	AnimationLoader animation_loader_;
    //Vector2D window_size_;
    bool quit_;
    std::list<Scene*> scene_list_;
    std::list<Layer*> interface_list_;
    uint32 reported_fps_, frames_since_reset_, last_fps_report_;

	Engine() : video_manager_(NULL), input_manager_(NULL), time_handler_(NULL), 
		audio_manager_(NULL), text_manager_(NULL), path_manager_(NULL),
		animation_loader_(new AnimationProtocol) {}
};

} // namespace ugdk

#endif

