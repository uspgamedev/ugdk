#include <ugdk/system/engine.h>

#include <string>
#include <algorithm>
#include <list>
#include "SDL.h"

#include <ugdk/action/scene.h>
#include <ugdk/audio/module.h>
#include <ugdk/audio/manager.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/manager.h>
#include <ugdk/graphic/textmanager.h>
#include <ugdk/input/module.h>
#include <ugdk/input/manager.h>
#include <ugdk/internal/sdleventhandler.h>
#include <ugdk/resource/module.h>
#include <ugdk/resource/manager.h>
#include <ugdk/time/module.h>
#include <ugdk/time/manager.h>
#include <ugdk/util/languagemanager.h>
#include <ugdk/script/scriptmanager.h>

namespace ugdk {
namespace system {

namespace {

// Attributes

graphic:: TextManager *        text_manager_;
      LanguageManager *    language_manager_;
bool quit_;
std::list<action::Scene*> scene_list_;
std::list<SceneFactory>   queued_scene_list_;
action::Scene*            previous_focused_scene_;
Configuration configuration_;

std::vector<const internal::SDLEventHandler*> sdlevent_handlers_;

void AddPendingScenes() {
    // Insert all queued Scenes at the end of the scene list.
    for(const SceneFactory& scene_factory : queued_scene_list_)
        scene_list_.push_back(scene_factory());
    queued_scene_list_.clear();
}

void DeleteFinishedScenes() {
    std::list<action::Scene*> to_delete;
    for(action::Scene* it : scene_list_)
        if(it->finished())
            to_delete.push_front(it);

    for(action::Scene* it : to_delete) {
        delete it;
        scene_list_.remove(it);
    }
}

void DefocusRoutine() {
    if(!previous_focused_scene_) return;
    // If we're adding any scenes, the current scene will lose it's focus.
    if(previous_focused_scene_->finished() || !queued_scene_list_.empty())
        previous_focused_scene_->DeFocus();
}

void FocusRoutine() {
    if(previous_focused_scene_ != CurrentScene())
        (previous_focused_scene_ = CurrentScene())->Focus();
}

void HandleSDLEvents() {
    ::SDL_Event sdlevent;
    while(SDL_PollEvent(&sdlevent)) {
        bool found = false;
        for(const internal::SDLEventHandler* handler : sdlevent_handlers_)
            if(handler->CanHandle(sdlevent)) {
                handler->Handle(sdlevent);
                found = true;
                break;
            }
        if(!found)
            printf("UGDK -- Event of type 0x%X received, but no known handler.\n", sdlevent.type);
    }
}

class SystemSDLEventHandler : public internal::SDLEventHandler {
  public:
    bool CanHandle(const ::SDL_Event& sdlevent) const { 
        return sdlevent.type == SDL_QUIT;
    }

    void Handle(const ::SDL_Event&) const {
        Quit();
    }

} system_sdlevent_handler;

}

graphic::TextManager *text_manager() {
    return text_manager_;
}

LanguageManager* language_manager() {
    return language_manager_;
}

std::string ResolvePath(const std::string& path) {
    return configuration_.base_path + path;
}

bool Initialize(const Configuration& configuration) {
    quit_ = false;
    SDL_Init(0);

    configuration_ = configuration;

    language_manager_ = new       LanguageManager(configuration.default_language);
    
    if(configuration.graphic_enabled)
        if(!graphic::Initialize(new graphic::Manager(graphic::VideoSettings(
            configuration.window_title,
            (configuration.window_icon.length() > 0) ? ResolvePath(configuration.window_icon) : "",
            configuration.window_resolution,
            configuration.fullscreen,
            true,
            false
        )))) return false;
    
    if(configuration.audio_enabled)
        if(!audio::Initialize(new audio::Manager))
            return false;
    
    if(configuration.input_enabled)
        if(!input::Initialize(new input::Manager))
            return false;
        else
            sdlevent_handlers_.push_back(input::manager()->sdlevent_handler());

    if(configuration.time_enabled)
        if(!time::Initialize(new time::Manager))
            return false;

    if(!resource::Initialize(new resource::Manager))
        return false;

    if(graphic::manager()) {
        text_manager_ = new graphic::TextManager;
        text_manager_->Initialize();
    }

    if (!SCRIPT_MANAGER()->Initialize())
        return false;

    scene_list_.clear();

    sdlevent_handlers_.push_back(&system_sdlevent_handler);

    return true;
}

void Run() {
    double delta_t;

    previous_focused_scene_ = nullptr;
    quit_ = false;
    while(!quit_) {
        // Pre-frame start logic
        DefocusRoutine();
        DeleteFinishedScenes();
        AddPendingScenes();
        if(CurrentScene() == nullptr)
            break;

        // Frame starts here!
        FocusRoutine();

        if(time::manager()) {
            time::manager()->Update();
            delta_t = (time::manager()->TimeDifference())/1000.0;
        } else
            delta_t = 0.0;

        // Over-compensation for lag causes bugs.
        delta_t = std::min(delta_t, 0.1);

        if(input::manager())
            input::manager()->Update();

        if(audio::manager())
            audio::manager()->Update();

        HandleSDLEvents();

        if (!quit_) {
            for(action::Scene* it : scene_list_)
                it->Update(delta_t);

            if(graphic::manager())
                graphic::manager()->Render(scene_list_);
        }
    }
    quit_ = true;
    for(action::Scene* it : scene_list_) {
        it->Finish();
        delete it;
    }
    scene_list_.clear();
}

void Release() {
    assert(quit_);
    sdlevent_handlers_.clear();

    if(text_manager_) {
        text_manager_->Release();
        delete text_manager_;
    }

    audio::Release();
    input::Release();
    resource::Release();
    time::Release();
    graphic::Release();

    SCRIPT_MANAGER()->Finalize();
    delete SCRIPT_MANAGER();

    SDL_Quit();
}

void PushScene(action::Scene* scene) {
    PushScene([scene]{ return scene; });
}

void PushScene(const SceneFactory& scene_factory) {
    queued_scene_list_.push_back(scene_factory);
}

action::Scene* CurrentScene() {
    return scene_list_.empty() ? nullptr : scene_list_.back();
}

const std::list<action::Scene*>& scene_list() {
    return scene_list_;
}

void Quit() {
    quit_ = true;
}

} // namespace system
} // namespace ugdk