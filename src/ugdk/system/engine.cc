#include <ugdk/system/engine.h>

#include <ugdk/system/config.h>

#include <ugdk/input/module.h>
#include <ugdk/graphic/module.h>
#include <ugdk/audio/module.h>
#include <ugdk/resource/module.h>
#include <ugdk/text/module.h>
#include <ugdk/time/module.h>
#include <ugdk/desktop/module.h>
#ifdef UGDK_3D_ENABLED
#include <ugdk/graphic/3D/module.h>
#endif

#include <ugdk/action/scene.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/internal/sdleventhandler.h>
#include <ugdk/util/languagemanager.h>
#include <ugdk/script/scriptmanager.h>
#include <ugdk/debug/profiler.h>
#include <ugdk/debug/log.h>
#include <ugdk/graphic/opengl/Exception.h>

#include <string>
#include <algorithm>
#include <list>
#include <fstream>
#include <cstring>
#include <cerrno>
#include "SDL.h"

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

LanguageManager* language_manager_;

std::list<std::unique_ptr<action::Scene >> scene_list_;
std::list<std::function<std::unique_ptr<action::Scene>()>> queued_scene_list_;
action::Scene*            previous_focused_scene_;
std::list<std::shared_ptr<const debug::SectionData>> profile_data_list_;
Configuration configuration_;

std::unordered_map<Uint32, const internal::SDLEventHandler*> sdlevent_mapper_;

bool ErrorLog(const std::string& err_msg) {
    debug::Log(debug::CRITICAL, err_msg);
    return false;
}

void AddPendingScenes() {
    // Insert all queued Scenes at the end of the scene list.
    for(const SceneFactory& scene_factory : queued_scene_list_)
        scene_list_.emplace_back(scene_factory());
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

class SDLQuitEventHandler : public internal::SDLEventHandler {
  public:
    std::unordered_set<Uint32> TypesHandled() const override {
        return { SDL_QUIT };
    }

    void Handle(const ::SDL_Event&) const override {
        Suspend();
    }

} system_sdlevent_handler;

} // namespace anon

LanguageManager* language_manager() {
    return language_manager_;
}

std::string ResolvePath(const std::string& path) {
    if(path.compare(0, configuration_.base_path.size(), configuration_.base_path) == 0)
        return path;
    return configuration_.base_path + path;
}

std::string GetFileContents(const std::string& filename) {
    std::ifstream in(ResolvePath(filename).c_str(), std::ios::in);
    if (in) {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return(contents);
    }
    auto error = errno;
    throw love::Exception("Unable to open file '%s'. Code %d, Reason: %s", filename.c_str(), error, std::strerror(error));
}

bool Initialize(const Configuration& configuration) {
    if (current_state_ != UGDKState::UNINITIALIZED)
        throw love::Exception("UGDK already initialized.");

    // Init SDL, but don't load any modules.
    SDL_Init(0);

    configuration_ = configuration;

    RegisterSDLHandler(&system_sdlevent_handler);

    language_manager_ = new LanguageManager(configuration.default_language);

    if(!configuration.windows_list.empty()) {
        if(!desktop::Initialize(new desktop::Manager))
            return ErrorLog("system::Initialize failed - desktop::Initialize returned false.");

        for(const auto& window_config : configuration.windows_list)
            desktop::manager()->CreateWindow(window_config);

        if(!graphic::Initialize(new graphic::Manager,
                                desktop::manager()->primary_window(),
                                configuration.canvas_size))
            return ErrorLog("system::Initialize failed - graphic::Initialize returned false.");

        if (!text::Initialize(new text::Manager))
            return ErrorLog("system::Initialize failed - text::Initialize returned false.");
    }
    
    if(configuration.ogre_enabled) {
#ifdef UGDK_3D_ENABLED
        if(!graphic::threed::Initialize(new graphic::threed::Manager(configuration.ogre_window_title)))
            return ErrorLog("system::Initialize failed - graphic::threed::Initialize returned false.");
#else
        return ErrorLog("system::Initialize failed - UGDK not compiled with 3D module.");
#endif
    }
    
    if(configuration.audio_enabled)
        if(!audio::Initialize(new audio::Manager))
            return ErrorLog("system::Initialize failed - audio::Initialize returned false.");
    
    if(configuration.input_enabled) {
        if(!input::Initialize(new input::Manager))
            return ErrorLog("system::Initialize failed - input::Initialize returned false.");
    }

    if(configuration.time_enabled)
        if(!time::Initialize(new time::Manager))
            return ErrorLog("system::Initialize failed - time::Initialize returned false.");

    if(!resource::Initialize(new resource::Manager))
        return ErrorLog("system::Initialize failed - resource::Initialize returned false.");

    if (!SCRIPT_MANAGER()->Initialize())
        return ErrorLog("system::Initialize failed - SCRIPT_MANAGER()->Initialize returned false.");

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
        throw love::Exception("UGDK not suspended.");
    current_state_ = UGDKState::RUNNING;

    while (current_state_ == UGDKState::RUNNING) {
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

        double delta_t;
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
    
        if (current_state_ != UGDKState::RUNNING)
            break;

        {
            debug::ProfileSection section("Frame");
            {
                debug::ProfileSection section("Update");
                for(auto& scene : scene_list_)
                    scene->Update(delta_t);
            }

            if(desktop::manager()) {
                debug::ProfileSection section("Render");
                if(graphic::manager()) {
                    graphic::Canvas canvas(graphic::manager()->screen());
                    canvas.Clear(Color(0.0, 0.0, 0.0, 0.0));
                    canvas.ChangeShaderProgram(graphic::manager()->shaders().current_shader());
                    for (auto& scene : scene_list_)
                        scene->Render(canvas);
                }
                desktop::manager()->PresentAll();
            }
            profile_data_list_.push_back(section.data());
        }
        while(profile_data_list_.size() > 10)
            profile_data_list_.pop_front();
    }
}

void Release() {
    if (current_state_ != UGDKState::SUSPENDED)
        throw love::Exception("UGDK not suspended.");
    current_state_ = UGDKState::RELEASED;

    DeregisterSDLHandler(&system_sdlevent_handler);

    audio::Release();
    input::Release();
    resource::Release();
    time::Release();
    graphic::Release();
#ifdef UGDK_3D_ENABLED
    graphic::threed::Release();
#endif
    desktop::Release();
    text::Release();

    SCRIPT_MANAGER()->Finalize();
    delete SCRIPT_MANAGER();

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
        throw 0;
    return *scene_list_.back();
}

const std::list<std::unique_ptr<action::Scene>>& scene_list() {
    return scene_list_;
}

const std::list<std::shared_ptr<const debug::SectionData>>& profile_data_list() {
    return profile_data_list_;
}

void Suspend() {
    if (current_state_ != UGDKState::RUNNING)
        throw love::Exception("UGDK not runing.");
    current_state_ = UGDKState::SUSPENDED;
}

void RegisterSDLHandler(const internal::SDLEventHandler* handler) {
    for (const auto& type : handler->TypesHandled()) {
        assert(sdlevent_mapper_.find(type) == sdlevent_mapper_.end());
        sdlevent_mapper_[type] = handler;
    }
}

void DeregisterSDLHandler(const internal::SDLEventHandler* handler) {
    for (const auto& type : handler->TypesHandled()) {
        assert(sdlevent_mapper_[type] == handler);
        sdlevent_mapper_.erase(type);
    }
}

} // namespace system
} // namespace ugdk
