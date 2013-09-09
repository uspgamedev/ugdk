#include <ugdk/action/scene.h>

#include <ugdk/action/entity.h>
#include <ugdk/audio/module.h>
#include <ugdk/audio/music.h>
#include <ugdk/graphic/node.h>

#include <algorithm>

namespace ugdk {
namespace action {

using std::list;

Scene::Scene() 
  : active_(true)
  , finished_(false)
  , visible_(true)
  , background_music_(nullptr) 
  , stops_previous_music_(true)
  , event_handler_(this)
{
    AddTask([&](double dt) {
        media_manager_.Update(dt);
    }, 0.4);

    // Update entities
    AddTask([&](double dt) {  
        for(auto it : entities_) it->Update(dt);
        return true;
    }, 0.5);

    // Remove to be deleted entities
    AddTask([&](double dt) {  
        entities_.remove_if([](Entity* e){ 
            bool is_dead = e->to_be_removed();
            if (is_dead) delete e;
            return is_dead;
        });
        return true;
    }, 0.9);

    // Flush add queue
    AddTask([&](double dt) {  
        while(!queued_entities_.empty()) {
            Entity* e = queued_entities_.front();
            this->AddEntity(e);
            queued_entities_.pop();
        }
        return true;
    }, 1.0);
}

Scene::~Scene() {
    //RemoveAllEntities(); Play safe... TODO: activate this and refactor users!
}

void Scene::Focus() {
    if(focus_callback_) focus_callback_(this);
    if(background_music_ != nullptr) { 
        if(!background_music_->IsPlaying())
            background_music_->PlayForever();
        else
            background_music_->Unpause();

    } else if(stops_previous_music_) {
        audio::Music* current_music = audio::manager()->CurrentMusic();
        if(current_music != nullptr)
            current_music->Stop();
    }
}

void Scene::DeFocus() {
    if(defocus_callback_) defocus_callback_(this);
}

void Scene::AddEntity(Entity *entity) { 
    entities_.push_back(entity);
    entity->OnSceneAdd(this);
}

void Scene::RemoveAllEntities() {
    std::list<Entity*>::iterator i;
    for (i = entities_.begin(); i != entities_.end(); ++i)
        delete (*i);
    entities_.clear();
}

void Scene::Update(double dt) {
    if(finished_ || !active_)
        return;
    TaskPlayer::Update(dt);
}

void Scene::Render(const graphic::Geometry& geometry, const graphic::VisualEffect& effect) const {
    if(!finished_ && visible_ && render_function_)
        render_function_(geometry, effect);
}

void Scene::End() {
    if(background_music_ != nullptr)
        background_music_->Pause();
}

} /* namespace action */
} /* namespace ugdk */
