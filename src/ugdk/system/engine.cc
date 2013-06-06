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
#include <ugdk/util/languagemanager.h>
#include <ugdk/script/scriptmanager.h>

namespace ugdk {
namespace system {

static    graphic:: TextManager *        text_manager_;
static          LanguageManager *    language_manager_;
static    bool quit_;
static    std::list<action::Scene*> scene_list_;
static    Configuration configuration_;

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
    bool result = true;

    quit_ = false;
    SDL_Init(0);

    configuration_ = configuration;

    language_manager_ = new       LanguageManager(configuration.default_language);
    
    if(configuration.graphic_enabled)
        result &= graphic::Initialize(new graphic::Manager(graphic::VideoSettings(
            configuration.window_title,
            (configuration.window_icon.length() > 0) ? ResolvePath(configuration.window_icon) : "",
            configuration.window_resolution,
            configuration.fullscreen,
            true,
            false
        )));
    
    if(configuration.audio_enabled)
        result &= audio::Initialize(new audio::Manager);
    
    if(configuration.input_enabled)
        result &= input::Initialize(new input::Manager);

    //TODO: make flag to disable module time
    result &= time::Initialize(new time::Manager);

    result &= resource::Initialize(new resource::Manager);
    
    if(graphic::manager()) {
        text_manager_ = new graphic:: TextManager();
        result &= text_manager_->Initialize();
    }

    if (!SCRIPT_MANAGER()->Initialize())
        puts("Failed to initialize script manager.");

    scene_list_.clear();
    return result;
}

static void DeleteFinishedScenes() {
    std::list<action::Scene*> to_delete;
    for(action::Scene* it : scene_list_)
        if(it->finished())
            to_delete.push_front(it);

    for(action::Scene* it : to_delete) {
        delete it;
        scene_list_.remove(it);
    }
}

void Run() {
    SDL_Event event;
    double delta_t;
    action::Scene* current_top_scene = nullptr;

    quit_ = false;
    while(!quit_) {
        if(current_top_scene && current_top_scene != CurrentScene()) {
            current_top_scene->DeFocus();
            (current_top_scene = CurrentScene())->Focus();
        }
        DeleteFinishedScenes();

        if (CurrentScene() == nullptr)
            Quit();
        else if(current_top_scene != CurrentScene())
            (current_top_scene = CurrentScene())->Focus();

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

        // tratamento de eventos
        while(SDL_PollEvent(&event)) {
            input::Key key;
            switch(event.type) {
                case SDL_QUIT:
                    Quit();
                    break;

                case SDL_KEYDOWN:
                    key = (input::Key)event.key.keysym.sym;
                    if(input::manager())
                        input::manager()->SimulateKeyPress(key);
                    break;

                case SDL_KEYUP:
                    key = (input::Key)event.key.keysym.sym;
                    if(input::manager())
                        input::manager()->SimulateKeyRelease(key);
                    break;

                default:
                    break;
            }
        }

        if (!quit_) {
            CurrentScene()->Update(delta_t);

            if(graphic::manager())
                graphic::manager()->Render(scene_list_);
        }
    }
    for(action::Scene* it : scene_list_) {
        it->Finish();
        delete it;
    }
    scene_list_.clear();
}

void Release() {
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
    scene_list_.push_back(scene);
}

action::Scene* CurrentScene() {
    return scene_list_.empty() ? nullptr : scene_list_.back();
}

void PopScene() {
    if(!scene_list_.empty()) scene_list_.pop_back();
}

void Quit() {
    quit_ = true;
}

} // namespace system
} // namespace ugdk