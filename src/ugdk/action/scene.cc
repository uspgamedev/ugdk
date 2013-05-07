#include <algorithm>

#include <ugdk/action/scene.h>

#include <ugdk/action/entity.h>
#include <ugdk/action/task.h>
#include <ugdk/base/engine.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/audio/music.h>
#include <ugdk/graphic/node.h>

namespace ugdk {

namespace action {

static bool entityIsToBeRemoved (const Entity* value) {
    bool is_dead = value->to_be_removed();
    if (is_dead)
        delete value;
    return is_dead;
}

static bool taskIsFinished (const Task* value) {
    bool is_dead = value->finished();
    if (is_dead)
        delete value;
    return is_dead;
}


using namespace std;

Scene::Scene() : finished_(false), background_music_(NULL), stops_previous_music_(true), 
    content_node_(new graphic::Node), interface_node_(new graphic::Node) {}

Scene::~Scene() {
    //RemoveAllEntities(); Play safe... TODO: activate this and refactor users!
    delete content_node_;
    delete interface_node_;
    for(TasksContainer::iterator it = tasks_.begin(); it != tasks_.end(); ++it)
        for(std::list<Task*>::iterator j = it->second.begin(); j != it->second.end(); ++j)
            delete (*j);
}

void Scene::Focus() {
    if(focus_callback_) focus_callback_(this);
    if(background_music_ != NULL) { 
        if(!background_music_->IsPlaying())
            background_music_->PlayForever();
        else
            background_music_->Unpause();

    } else if(stops_previous_music_) {
        audio::Music* current_music = AUDIO_MANAGER()->CurrentMusic();
        if(current_music != NULL)
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

void Scene::AddTask(Task *task) {
    tasks_[task->priority()].push_back(task);
}

void Scene::Update(double delta_t) {
    UpdateEntities(delta_t);
    media_manager_.Update(delta_t);
    UpdateTasks(delta_t);
    DeleteToBeRemovedEntities();
    DeleteFinishedTasks();
    FlushEntityQueue();
}

void Scene::End() {
    if(background_music_ != NULL)
        background_music_->Pause();
}

void Scene::UpdateEntities(double delta_t) {
    for(std::list<Entity*>::iterator it = entities_.begin(); it != entities_.end(); ++it)
        (*it)->Update(delta_t);
}

void Scene::UpdateTasks(double delta_t) {
    for(TasksContainer::iterator it = tasks_.begin(); it != tasks_.end(); ++it)
        for(std::list<Task*>::iterator j = it->second.begin(); j != it->second.end(); ++j)
            (**j)(delta_t);
}

void Scene::DeleteToBeRemovedEntities() {
    entities_.remove_if(entityIsToBeRemoved);
}

void Scene::DeleteFinishedTasks() {
    for(TasksContainer::iterator it = tasks_.begin(); it != tasks_.end(); ++it)
        it->second.remove_if(taskIsFinished);
}

void Scene::FlushEntityQueue() {
    while(!queued_entities_.empty()) {
        Entity* e = queued_entities_.front();
        AddEntity(e);
        queued_entities_.pop();
    }
}

} /* namespace action */

} /* namespace ugdk */
