#include <ugdk/action/scene.h>
#include <ugdk/base/engine.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/audio/music.h>

namespace ugdk {

using namespace std;

Scene::~Scene() {
    if (layers_.size() == 0) return;
    list<Layer*>::iterator it = layers_.begin();
    while (it != layers_.end()) {
        delete (*it);
        ++it;
    }

    delete root_node_;
}

void Scene::Focus() {
    if(background_music_ != NULL) { 
        if(!background_music_->IsPlaying())
            background_music_->PlayForever();
        else
            background_music_->Unpause();
    } else if(stops_previous_music_) {
        Music* current_music = AUDIO_MANAGER()->CurrentMusic();
        if(current_music != NULL)
            current_music->Stop();
    }
}

void Scene::Update(float delta_t) {

    if (layers_.size() == 0) return;

    list<Layer*>::iterator it = layers_.begin();

    while (it != layers_.end()) {
        (*it)->Update(delta_t);
        ++it;
    }
}

void Scene::Render() {
    if (visible()) {
        list<Layer*>::iterator it = layers_.begin();
        while (it != layers_.end()) {
            (*it)->Render();
            ++it;
        }
    }
}

void Scene::RenderLight() {
    if (visible()) {
        list<Layer*>::iterator it = layers_.begin();
        while (it != layers_.end()) {
            (*it)->RenderLight();
            ++it;
        }
    }
}

void Scene::End() {
    if(background_music_ != NULL)
        background_music_->Pause();
}

}
