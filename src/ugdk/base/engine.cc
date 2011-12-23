#include <string>
#include <algorithm>
#include <SDL/SDL.h>
#include <ugdk/base/engine.h>
#include <ugdk/action/scene.h>
#include <ugdk/input/inputmanager.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/time/timehandler.h>
#include <ugdk/graphic/textmanager.h>
#include <ugdk/util/pathmanager.h>
#include <ugdk/util/animationparser.h>

using namespace std;

namespace ugdk {


Vector2D Engine::window_size() {
    return video_manager_->video_size();
}

bool Engine::Initialize(string windowTitle, Vector2D windowSize, 
						bool fullscreen, std::string base_path, std::string icon) {
    quit_ = false;
    video_manager_ = new VideoManager();
    SDL_Init(SDL_INIT_EVERYTHING);
    video_manager_->Initialize(windowTitle, windowSize, fullscreen, icon);
    input_manager_ = new InputManager();
    time_handler_ = new TimeHandler();
    audio_manager_ = new AudioManager();
    audio_manager_->Initialize();
    text_manager_ = new TextManager();
    text_manager_->Initialize();
	path_manager_ = new PathManager(base_path);
    scene_list_.clear();
    interface_list_.clear();

    frames_since_reset_ = reported_fps_ = 0;
    if(time_handler_ != NULL)
        last_fps_report_ = time_handler_->TimeElapsed();

    return (time_handler_ != NULL);
}

void Engine::DeleteFinishedScenes() {
    std::list<Scene*> to_delete;
    for(std::list<Scene*>::iterator it = scene_list_.begin(); it != scene_list_.end(); ++it)
        if((*it)->finished())
            to_delete.push_front(*it);

    for(std::list<Scene*>::iterator it = to_delete.begin(); it != to_delete.end(); ++it) {
        delete (*it);
        scene_list_.remove(*it);
    }
}



void Engine::Run() {
    Key key;
    SDL_Event event;
    float delta_t, total_fps = 0;
    Scene* current_top_scene = NULL;

    quit_ = false;
    while(!quit_) {
        if(current_top_scene && current_top_scene != CurrentScene()) {
            current_top_scene->DeFocus();
            (current_top_scene = CurrentScene())->Focus();
        }
        DeleteFinishedScenes();
        if(current_top_scene != CurrentScene())
            (current_top_scene = CurrentScene())->Focus();

        // gerenciamento das cenas
        if (scene_list_.size() == 0) {
            quit();
        }

        // gerenciamento de tempo
        time_handler_->Update();
        delta_t = (time_handler_->TimeDifference())/1000.0f;

        // Verifica se o FPS nao esta baixo demais.
        // Impede que os personagens atravessem paredes.
        delta_t = min(delta_t, 0.1f);

        // gerenciador de input
        input_manager()->Update(delta_t);

        // gerenciamento de audio
        audio_manager()->Update();

        // tratamento de eventos
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    quit();
                    break;

                case SDL_KEYDOWN:
                    key = (Key)event.key.keysym.sym;
                    input_manager()->SimulateKeyPress(key);
                    break;

                case SDL_KEYUP:
                    key = (Key)event.key.keysym.sym;
                    input_manager()->SimulateKeyRelease(key);
                    break;

                default:
                    break;
            }
        }

        if (!quit_) {
            CurrentScene()->Update(delta_t);
            for (std::list<Layer*>::iterator it = interface_list_.begin(); 
                it != interface_list_.end(); ++it)
                (*it)->Update(delta_t);

            // Sends the scene list to the videomanager, who handles everything 
            // needed to draw
            video_manager_->Render(scene_list_, interface_list_);

            ++frames_since_reset_;
            total_fps += 1.0f/delta_t;
            if(frames_since_reset_ == 10) {
                reported_fps_ = static_cast<int>(total_fps/10.0f);
                frames_since_reset_ = 0;
                total_fps = 0;
            }
        }
    }
    for(std::list<Scene*>::iterator it = scene_list_.begin(); it != scene_list_.end(); ++it) {
        (*it)->Finish();
        delete (*it);
    }
    scene_list_.clear();
}

void Engine::Release() {
    delete time_handler_;
    delete input_manager_;

    audio_manager()->Release();
    delete audio_manager_;

	text_manager_->Release();
	delete text_manager_;

    video_manager()->Release();
    delete video_manager_;

    animation_loader_.ClearCache();

    SDL_Quit();
}

void Engine::PushScene(Scene* scene) {
    scene_list_.push_back(scene);
}

Scene* Engine::CurrentScene() const {
    return scene_list_.back();
}

void Engine::PopScene() {
    scene_list_.pop_back();
}

void Engine::PushInterface(Layer* layer) {
    interface_list_.push_back(layer);
}
void Engine::RemoveInterface(Layer *layer) {
    interface_list_.remove(layer);
}

}
