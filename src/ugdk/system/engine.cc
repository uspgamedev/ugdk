#include <ugdk/system/engine.h>

#include <string>
#include <algorithm>
#include "SDL.h"

#include <ugdk/action/scene.h>
#include <ugdk/audio/module.h>
#include <ugdk/audio/manager.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/manager.h>
#include <ugdk/graphic/textmanager.h>
#include <ugdk/input/module.h>
#include <ugdk/input/manager.h>
#include <ugdk/resource/module.h>
#include <ugdk/resource/manager.h>
#include <ugdk/time/module.h>
#include <ugdk/time/manager.h>
#include <ugdk/util/pathmanager.h>
#include <ugdk/util/languagemanager.h>
#include <ugdk/script/scriptmanager.h>

using namespace std;

namespace ugdk {

using namespace graphic;
using namespace input;

Engine* Engine::reference_ = nullptr;

ugdk::math::Vector2D Engine::window_size() {
    return video_manager_->video_size();
}

bool Engine::Initialize(const Configuration& configuration) {
    quit_ = false;
    SDL_Init(0);

    path_manager_     = new           PathManager(configuration.base_path);
    language_manager_ = new       LanguageManager(configuration.default_language);
    
    std::string icon_path = (configuration.window_icon.length() > 0) ? path_manager_->ResolvePath(configuration.window_icon) : "";

    graphic::Manager* graphic_manager = new graphic::Manager;
    if(graphic_manager) {
        graphic_manager->Configure(configuration.window_title, configuration.window_size, 
                        graphic::VideoSettings(configuration.fullscreen, true, false), icon_path);
        graphic::Initialize(graphic_manager);
    }
    audio::Initialize(new audio::Manager);
    input::Initialize(new input::Manager);
    resource::Initialize(new resource::Manager);
    time::Initialize(new time::Manager);
    
    text_manager_ = new graphic:: TextManager();
    text_manager_->Initialize();

    if (!SCRIPT_MANAGER()->Initialize())
        puts("Failed to initialize script manager.");

    scene_list_.clear();

    frames_since_reset_ = reported_fps_ = 0;
    if(time::manager() != nullptr)
        last_fps_report_ = time::manager()->TimeElapsed();

    return (time::manager() != nullptr);
}

void Engine::DeleteFinishedScenes() {
    std::list<action::Scene*> to_delete;
    for(action::Scene* it : scene_list_)
        if(it->finished())
            to_delete.push_front(it);

    for(action::Scene* it : to_delete) {
        delete it;
        scene_list_.remove(it);
    }
}



void Engine::Run() {
    Key key;
    SDL_Event event;
    double delta_t, total_fps = 0;
    action::Scene* current_top_scene = nullptr;

    quit_ = false;
    while(!quit_) {
        if(current_top_scene && current_top_scene != CurrentScene()) {
            current_top_scene->DeFocus();
            (current_top_scene = CurrentScene())->Focus();
        }
        DeleteFinishedScenes();

        if (CurrentScene() == nullptr)
            quit();
        else if(current_top_scene != CurrentScene())
            (current_top_scene = CurrentScene())->Focus();

        if(time::manager()) {
            time::manager()->Update();
            delta_t = (time::manager()->TimeDifference())/1000.0;
        } else
            delta_t = 0.0;

        // Over-compensation for lag causes bugs.
        delta_t = min(delta_t, 0.1);

        if(input::manager())
            input::manager()->Update();

        if(audio::manager())
            audio::manager()->Update();

        // tratamento de eventos
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    quit();
                    break;

                case SDL_KEYDOWN:
                    key = (Key)event.key.keysym.sym;
                    if(input::manager())
                        input::manager()->SimulateKeyPress(key);
                    break;

                case SDL_KEYUP:
                    key = (Key)event.key.keysym.sym;
                    if(input::manager())
                        input::manager()->SimulateKeyRelease(key);
                    break;

                default:
                    break;
            }
        }

        if (!quit_) {
            CurrentScene()->Update(delta_t);

            // Sends the scene list to the videomanager, who handles everything 
            // needed to draw
            video_manager_->Render(scene_list_);

            ++frames_since_reset_;
            total_fps += 1.0/delta_t;
            if(frames_since_reset_ == 10) {
                reported_fps_ = static_cast<int>(total_fps/10.0);
                frames_since_reset_ = 0;
                total_fps = 0;
            }
        }
    }
    for(action::Scene* it : scene_list_) {
        it->Finish();
        delete it;
    }
    scene_list_.clear();
}

void Engine::Release() {
    audio::Release();
    input::Release();
    resource::Release();
    time::Release();

    text_manager_->Release();
    delete text_manager_;

    video_manager()->Release();
    delete video_manager_;

    SCRIPT_MANAGER()->Finalize();
    delete SCRIPT_MANAGER();

    SDL_Quit();
}

void Engine::PushScene(action::Scene* scene) {
    scene_list_.push_back(scene);
}

action::Scene* Engine::CurrentScene() const {
    return scene_list_.empty() ? nullptr : scene_list_.back();
}

void Engine::PopScene() {
    if(!scene_list_.empty()) scene_list_.pop_back();
}

}
