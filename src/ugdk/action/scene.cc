#include <algorithm>

#include <ugdk/action/scene.h>

#include <ugdk/action/entity.h>
#include <ugdk/system/engine.h>
#include <ugdk/audio/module.h>
#include <ugdk/audio/music.h>
#include <ugdk/graphic/node.h>

namespace ugdk {
namespace action {

using std::list;

/*static bool entityIsToBeRemoved (const Entity* value) {
    bool is_dead = value->to_be_removed();
    if (is_dead)
        delete value;
    return is_dead;
}*/

Scene::Scene() 
  : finished_(false), 
    background_music_(nullptr), 
    stops_previous_music_(true), 
    content_node_(new graphic::Node), 
    interface_node_(new graphic::Node) {

        tasks_.emplace_back(0.4, [&](double dt) {
            media_manager_.Update(dt);
            return true;
        });

        // Update entities
        tasks_.emplace_back(0.5, [&](double dt) {  
            for(auto it : entities_) it->Update(dt);
            return true;
        });

        // Remove to be deleted entities
        tasks_.emplace_back(0.9, [&](double dt) {  
            entities_.remove_if([](Entity* e){ 
                bool is_dead = e->to_be_removed();
                if (is_dead) delete e;
                return is_dead;
            });
            return true;
        });

        // Flush add queue
        tasks_.emplace_back(1.0, [&](double dt) {  
            while(!queued_entities_.empty()) {
                Entity* e = queued_entities_.front();
                this->AddEntity(e);
                queued_entities_.pop();
            }
            return true;
        });
}

Scene::~Scene() {
    //RemoveAllEntities(); Play safe... TODO: activate this and refactor users!
    delete content_node_;
    delete interface_node_;
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

void Scene::AddTask(const Task& task, double priority) {
    tasks_.merge(list<OrderedTask>(1, OrderedTask(priority, task)));
}

void Scene::Update(double dt) {
    // For each task, do. Note the tasks list is already ordered by the priorities.
    tasks_.remove_if([dt](OrderedTask& otask) {
        // Calls the task, and removes it from the list if the task returned false.
        return !otask.task(dt);
    });
}

void Scene::End() {
    if(background_music_ != nullptr)
        background_music_->Pause();
}

} /* namespace action */
} /* namespace ugdk */
