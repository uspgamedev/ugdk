#include <ugdk/system/engine.h>

#include <ugdk/system/config.h>

#include <ugdk/input/module.h>
#include <ugdk/audio/module.h>
#include <ugdk/resource/module.h>
#include <ugdk/time/module.h>
#include <ugdk/filesystem/module.h>
#include <ugdk/desktop/module.h>
#include <ugdk/desktop/manager.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/text/manager.h>
#include <ugdk/text/module.h>

#include <ugdk/action/scene.h>
#include <ugdk/debug/profiler.h>
#include <ugdk/debug/log.h>
#include <ugdk/system/exceptions.h>

#include <system/sdleventhandler.h>

#include "SDL.h"

#include <algorithm>
#include <cstring>
#include <cerrno>
#include <memory>
#include <fstream>
#include <string>
#include <list>
#include <iostream>

namespace ugdk {
namespace system {

namespace {



enum class UGDKState {
    UNINITIALIZED,
    SUSPENDED,
    RUNNING,
    RELEASED,
};

// Attributes

UGDKState current_state_ = UGDKState::UNINITIALIZED;

std::list<std::unique_ptr<action::Scene >> scene_list_;
std::list<std::function<std::unique_ptr<action::Scene>()>> queued_scene_list_;
action::Scene*            previous_focused_scene_;
std::list<std::shared_ptr<const debug::SectionData>> profile_data_list_;
std::unique_ptr<EventHandler> global_event_handler_;
Configuration configuration_;

std::unordered_map<Uint32, const SDLEventHandler*> sdlevent_mapper_;

bool ErrorLog(const std::string& err_msg) {
    debug::Log(debug::CRITICAL, err_msg);
    return false;
}

void AddPendingScenes() {
    // Insert all queued Scenes at the end of the scene list.
    for(const SceneFactory& scene_factory : queued_scene_list_) {
        scene_list_.emplace_back(scene_factory());
        scene_list_.back()->OnPushed(scene_list_.size()-1);
    }
    queued_scene_list_.clear();
}

void DeleteFinishedScenes() {
    scene_list_.remove_if([](std::unique_ptr<action::Scene>& scene) {
        return scene->finished();
    });
}

void DefocusRoutine() {
    if(!previous_focused_scene_) return;
    // If we're adding any scenes, the current scene will lose it's focus.
    if(previous_focused_scene_->finished() || !queued_scene_list_.empty())
        previous_focused_scene_->DeFocus();
}

void FocusRoutine() {
    if(previous_focused_scene_ != scene_list_.back().get())
        (previous_focused_scene_ = scene_list_.back().get())->Focus();
}

void HandleSDLEvents() {
    ::SDL_Event sdlevent;
    while(SDL_PollEvent(&sdlevent)) {

        auto mapper = sdlevent_mapper_.find(sdlevent.type);
        if (mapper != sdlevent_mapper_.end()) {
            mapper->second->Handle(sdlevent);

        } else {
            debug::DebugLog(debug::LogLevel::INFO, "UGDK",
                            "Event of type ", sdlevent.type, " received, but no known handler.");
        }
    }
}

class SDLQuitEventHandler : public SDLEventHandler {
  public:
    std::unordered_set<Uint32> TypesHandled() const override {
        return { SDL_QUIT };
    }

    void Handle(const ::SDL_Event&) const override {
        Suspend();
    }

} system_sdlevent_handler;

} // namespace anon

bool Initialize(const Configuration& configuration) {
    if (current_state_ != UGDKState::UNINITIALIZED)
        throw BaseException("UGDK already initialized.");

    // Init SDL, but don't load any modules.
    SDL_Init(0);

    configuration_ = configuration;
    global_event_handler_.reset(new EventHandler);

    RegisterSDLHandler(&system_sdlevent_handler);

    if (filesystem::Initialize(std::unique_ptr<filesystem::Manager>(new filesystem::Manager )))
        filesystem::manager().AddSearchPath(configuration_.base_path);

    if(!configuration.windows_list.empty()) {
        std::unique_ptr<desktop::Manager> deskmanager(new desktop::Manager);
        if(!desktop::Initialize(std::move(deskmanager)))
            return ErrorLog("system::Initialize failed - desktop::Initialize returned false.");
        for(const auto& window_config : configuration.windows_list)
            desktop::manager().CreateWindow(window_config);
    }

    if(configuration.audio_enabled)
        if(!audio::Initialize(std::unique_ptr<audio::Manager>(new audio::Manager )))
            return ErrorLog("system::Initialize failed - audio::Initialize returned false.");

    if(configuration.input_enabled) {
        if(!input::Initialize(std::unique_ptr<input::Manager>(new input::Manager )))
            return ErrorLog("system::Initialize failed - input::Initialize returned false.");
    }

    if(configuration.time_enabled)
        if(!time::Initialize(std::unique_ptr<time::Manager>(new time::Manager )))
            return ErrorLog("system::Initialize failed - time::Initialize returned false.");


    if (!text::Initialize(std::make_unique<text::Manager>(configuration.default_language)))
        return ErrorLog("system::Initialize failed - text::Initialize returned false.");

    if(!resource::Initialize(std::unique_ptr<resource::Manager>(new resource::Manager )))
        return ErrorLog("system::Initialize failed - resource::Initialize returned false.");

    if(!graphic::Initialize(std::make_unique<graphic::Manager>(),
       desktop::manager().primary_window(), configuration_.windows_list.front().canvas_size))
        return ErrorLog("system::Initialize failed - graphic::Initialize returned false.");

    previous_focused_scene_ = nullptr;
    current_state_ = UGDKState::SUSPENDED;

    return true;
}

bool Initialize() {
    Configuration defaults;
    return Initialize(defaults);
}

void Run() {
    if (current_state_ != UGDKState::SUSPENDED)
        throw BaseException("UGDK not suspended.");
    current_state_ = UGDKState::RUNNING;

    while (current_state_ == UGDKState::RUNNING) {
        // Start the frame time as first thing
        double delta_t;
        if (time::is_active()) {
            time::Manager& time_manager = time::manager();
            time_manager.Update();
            delta_t = (time_manager.TimeDifference()) / 1000.0;
        } else {
            delta_t = 0.0;
        }

        // Over-compensation for lag causes bugs.
        delta_t = std::min(delta_t, 0.1);

        // Pre-frame start logic
        DefocusRoutine();
        DeleteFinishedScenes();
        AddPendingScenes();

        if (scene_list_.empty()) {
            current_state_ = UGDKState::SUSPENDED;
            previous_focused_scene_ = nullptr;
            break;
        }

        // Frame starts here!
        FocusRoutine();

        if(input::is_active())
            input::manager().Update();

        if(audio::is_active())
            audio::manager().Update();

        HandleSDLEvents();

        if (current_state_ != UGDKState::RUNNING)
            break;

        {
            debug::ProfileSection frame_section("Frame");
            {
                debug::ProfileSection update_section("Update");
                for(auto& scene : scene_list_)
                    scene->Update(delta_t);
            }

            if(desktop::is_active()) {
                debug::ProfileSection render_section("Render");
                
                auto &manager = graphic::manager();
                std::vector<graphic::Canvas*> canvases;
                
                //std::cout << manager.num_screens() << " Screens" << std::endl;

                for (uint32_t i=0; i<manager.num_screens(); i++)
                    canvases.push_back(new graphic::Canvas(manager.screen(i)));
                
                for(auto& scene : scene_list_)
                    if (scene->visible())
                        scene->Render(canvases);
                //desktop::manager().PresentAll();

                for (auto canvas_ptr : canvases) {
                    delete canvas_ptr;
                }
            }
            profile_data_list_.push_back(frame_section.data());
        }
        while(profile_data_list_.size() > 10)
            profile_data_list_.pop_front();

        if (configuration_.limit_frame_time_with_sleep && time::is_active()) {
            auto frame_time = time::manager().TimeElapsedInFrame();
            if (frame_time < configuration_.target_frame_time)
                SDL_Delay(configuration_.target_frame_time - frame_time);
        }
    }
}

void Suspend() {
    if (current_state_ != UGDKState::RUNNING)
        throw BaseException("UGDK not runing.");
    current_state_ = UGDKState::SUSPENDED;
}

void Release() {
    if (current_state_ != UGDKState::SUSPENDED)
        throw BaseException("UGDK not suspended.");
    current_state_ = UGDKState::RELEASED;

    for (; !scene_list_.empty(); scene_list_.pop_back()) {
        scene_list_.back()->Finish();
    }
    queued_scene_list_.clear();

    DeregisterSDLHandler(&system_sdlevent_handler);
    global_event_handler_.reset();

    audio::Release();
    input::Release();
    resource::Release();
    time::Release();

    desktop::Release();
    filesystem::Release();

    assert(sdlevent_mapper_.empty());

    SDL_Quit();
}

void PushScene(std::unique_ptr<action::Scene> scene) {
    action::Scene* ptr = scene.release();
    queued_scene_list_.emplace_back([ptr] { return std::unique_ptr<action::Scene>(ptr); });
}

void PushSceneFactory(const std::function<std::unique_ptr<action::Scene>()>& scene_factory) {
    queued_scene_list_.push_back(scene_factory);
}

action::Scene& CurrentScene() {
    if (scene_list_.empty())
        throw InvalidOperation("Attempting to get CurrentScene with empty scene list.");
    return *scene_list_.back();
}

const std::list<std::unique_ptr<action::Scene>>& scene_list() {
    return scene_list_;
}

const std::list<std::shared_ptr<const debug::SectionData>>& profile_data_list() {
    return profile_data_list_;
}

const Configuration& CurrentConfiguration() {
    return configuration_;
}

EventHandler& GlobalEventHandler() {
    return *global_event_handler_;
}

EventHandler& GetCurrentSceneEventHandler() {
    return CurrentScene().event_handler();
}

void RegisterSDLHandler(const SDLEventHandler* handler) {
    for (const auto& type : handler->TypesHandled()) {
        assert(sdlevent_mapper_.find(type) == sdlevent_mapper_.end());
        sdlevent_mapper_[type] = handler;
    }
}

void DeregisterSDLHandler(const SDLEventHandler* handler) {
    for (const auto& type : handler->TypesHandled()) {
        assert(sdlevent_mapper_[type] == handler);
        sdlevent_mapper_.erase(type);
    }
}

} // namespace system
} // namespace ugdk
