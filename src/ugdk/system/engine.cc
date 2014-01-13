#include <ugdk/system/engine.h>

#include <ugdk/input/module.h>
#include <ugdk/graphic/module.h>
#include <ugdk/audio/module.h>
#include <ugdk/resource/module.h>
#include <ugdk/time/module.h>
#include <ugdk/desktop/module.h>

#include <ugdk/action/scene.h>
#include <ugdk/graphic/textmanager.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/internal/sdleventhandler.h>
#include <ugdk/util/languagemanager.h>
#include <ugdk/script/scriptmanager.h>
#include <ugdk/system/config.h>
#include <ugdk/debug/profiler.h>

#include <string>
#include <algorithm>
#include <list>
#include "SDL.h"

namespace ugdk {
namespace system {

namespace {

// Attributes

graphic:: TextManager *        text_manager_;
      LanguageManager *    language_manager_;

bool                      quit_;
std::list<action::Scene*> scene_list_;
std::list<SceneFactory>   queued_scene_list_;
std::list<std::shared_ptr<const debug::SectionData>> profile_data_list_;
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
    if(path.compare(0, configuration_.base_path.size(), configuration_.base_path) == 0)
        return path;
    return configuration_.base_path + path;
}

bool Initialize(const Configuration& configuration) {
    quit_ = false;
    scene_list_.clear();

    // Init SDL, but don't load any modules.
    SDL_Init(0);

    configuration_ = configuration;

    sdlevent_handlers_.push_back(&system_sdlevent_handler);

    language_manager_ = new LanguageManager(configuration.default_language);

    if(!configuration.windows_list.empty()) {
        if(!desktop::Initialize(new desktop::Manager))
            return false;

        for(const auto& window_config : configuration.windows_list)
            desktop::manager()->CreateWindow(window_config);

        if(!graphic::Initialize(new graphic::Manager(
                                    desktop::manager()->primary_window(),
                                    configuration.canvas_size)))
            return false;
    }
    
    if(configuration.audio_enabled)
        if(!audio::Initialize(new audio::Manager))
            return false;
    
    if(configuration.input_enabled) {
        if(!input::Initialize(new input::Manager))
            return false;
        sdlevent_handlers_.push_back(input::manager()->sdlevent_handler());
    }

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

    return true;
}

bool Initialize() {
    Configuration defaults;
    return Initialize(defaults);
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
            debug::ProfileSection section("Frame");
            {
                debug::ProfileSection section("Update");
                for(action::Scene* it : scene_list_)
                    it->Update(delta_t);
            }

            if(desktop::manager()) {
                debug::ProfileSection section("Render");
                if(graphic::manager()) {
                    auto canvas = graphic::manager()->canvas();
                    canvas->Clear();
                    for(action::Scene* it : scene_list_)
                        it->Render(*canvas);
                }
                desktop::manager()->PresentAll();
            }
            profile_data_list_.push_back(section.data());
        }
        while(profile_data_list_.size() > 10)
            profile_data_list_.pop_front();
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
    desktop::Release();

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

const std::list< std::shared_ptr<const debug::SectionData> >& profile_data_list() {
    return profile_data_list_;
}

void Quit() {
    quit_ = true;
}

} // namespace system
} // namespace ugdk