#include <ugdk/action/scene.h>

#include <ugdk/action/entity.h>
#include <ugdk/base/engine.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/audio/music.h>
#include <ugdk/graphic/node.h>

namespace ugdk {

using namespace std;

Scene::Scene() : finished_(false), background_music_(NULL), stops_previous_music_(true), root_node_(new graphic::Node) {}

Scene::~Scene() {
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

void Scene::Update(double delta_t) {
    for(std::list<Entity*>::iterator it = entities_.begin(); it != entities_.end(); ++it)
        (*it)->Update(delta_t);
}

bool Scene::visible() const { return root_node_->modifier()->visible(); }
void Scene::set_visible(bool set) { root_node_->modifier()->set_visible(set); }

void Scene::End() {
    if(background_music_ != NULL)
        background_music_->Pause();
}

}
