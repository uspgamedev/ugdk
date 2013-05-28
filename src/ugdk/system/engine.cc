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

static    graphic:: TextManager *        text_manager_;
static	          PathManager *        path_manager_;
static          LanguageManager *    language_manager_;
static    bool quit_;
static    std::list<action::Scene*> scene_list_;

graphic::TextManager *text_manager() {
    return text_manager_;
}

PathManager *path_manager() {
    return path_manager_;
}

LanguageManager* language_manager() {
    return language_manager_;
}

bool Initialize(const Configuration& configuration) {
    quit_ = false;
    SDL_Init(0);

    path_manager_     = new           PathManager(configuration.base_path);
    language_manager_ = new       LanguageManager(configuration.default_language);
    
    if(configuration.graphic_enabled) {
        graphic::Manager* graphic_manager = new graphic::Manager;
        if(graphic_manager) {
            std::string icon_path = (configuration.window_icon.length() > 0) ? 
                                        path_manager_->ResolvePath(configuration.window_icon) : "";
            graphic_manager->Configure(configuration.window_title, configuration.window_size, 
                            graphic::VideoSettings(configuration.fullscreen, true, false), icon_path);
            graphic::Initialize(graphic_manager);
        }
    }
    if(configuration.audio_enabled)
        audio::Initialize(new audio::Manager);
    
    if(configuration.input_enabled)
        input::Initialize(new input::Manager);

    resource::Initialize(new resource::Manager);
    time::Initialize(new time::Manager);
    
    if(graphic::manager()) {
        text_manager_ = new graphic:: TextManager();
        text_manager_->Initialize();
    }

    if (!SCRIPT_MANAGER()->Initialize())
        puts("Failed to initialize script manager.");

    scene_list_.clear();
    return (time::manager() != nullptr);
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
        delta_t = min(delta_t, 0.1);

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

}
