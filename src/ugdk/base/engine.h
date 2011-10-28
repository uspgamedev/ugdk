#ifndef UGDK_BASE_ENGINE_H_
#define UGDK_BASE_ENGINE_H_

#include <vector>
#include <list>
#include <string>
#include <ugdk/math/vector2D.h>
#include <ugdk/action/scene.h>
#include <ugdk/util/gdd/cachedloader.h>
#include <ugdk/util/animationprotocol.h>

using std::vector;

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

class Engine {
  public:
    // Engine e' um singleton. Use Engine::reference()
    // para obter um ponteiro para o objeto desta classe.
    static Engine* reference() { static Engine *r = NULL;
                                 return r ? r : r = new Engine; }

    VideoManager *video_manager() { return video_manager_; }
    InputManager *input_manager() { return input_manager_; }
    TimeHandler *time_handler() { return time_handler_; }
    AudioManager *audio_manager() { return audio_manager_; }
    TextManager *text_manager() { return text_manager_; }
	PathManager *path_manager() { return path_manager_; }

	AnimationLoader& animation_loader() { return animation_loader_; }
    Vector2D window_size();

    // Gerenciamento do motor
	bool Initialize(std::string windowTitle, Vector2D windowSize, 
		bool fullscreen,
		std::string base_path = std::string("./"),
		std::string icon = std::string());

    void Run();
    void Release();

    // Gerenciamento de cenas
    void PushScene(Scene* scene);
    Scene* CurrentScene() const;
    void PopScene();

    // TODO: SERIOUSLY FIX THIS. Big fat ugly code to allow some special 
    // layers to be unnafected by the light system. Meant for user interface.
    void PushInterface(Layer* layer);
    void RemoveInterface(Layer *layer);

    uint32 current_fps() { return reported_fps_; }

    // Saida do motor
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
    vector<Scene*> scene_list_;
    std::list<Layer*> interface_list_;
    uint32 reported_fps_, frames_since_reset_, last_fps_report_;

	Engine() : video_manager_(NULL), input_manager_(NULL), time_handler_(NULL), 
		audio_manager_(NULL), text_manager_(NULL), path_manager_(NULL),
		animation_loader_(new AnimationProtocol) {}
};

} // namespace ugdk

#endif

