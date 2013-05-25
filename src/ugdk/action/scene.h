#ifndef UGDK_ACTION_SCENE_H_
#define UGDK_ACTION_SCENE_H_

#include <functional>
#include <list>
#include <queue>
#include <ugdk/action.h>
#include <ugdk/audio.h>
#include <ugdk/graphic.h>
#include <ugdk/base/types.h>
#include <ugdk/action/mediamanager.h>

namespace ugdk {
namespace action {

/**
   @class Scene
   @brief A game scene, or screen.

   Represents a scene, containing various graphical layers and sprites.
   A scene is game screen, such as a menu, or the playable part
   itself.
*/
class Scene {
  public:
    Scene();
      
    virtual ~Scene();

    /// Method called when this Scene arrives on the top of the Scene stack.
    virtual void Focus();

    /// Method called when this Scene leaves the top of the Scene stack.
    virtual void DeFocus();

    /// Adds an Entity to the scene.
    void AddEntity(Entity *entity);

    /// Removes the specified Entity from the scene.
    void RemoveEntity(Entity *entity) { entities_.remove(entity); }

    /// Will be added at the end of the 
    void QueuedAddEntity(Entity *entity) { queued_entities_.push(entity); }

    void RemoveAllEntities();

    /// Adds a Task to the scene.
    void AddTask(const Task& task, int priority = 0);

    /// Finishes the scene.
    void Finish() { End(); finished_ = true; }

    /// Logical update of the scene.
    /**
       @param delta_t Time in seconds since last update
    */
    void Update(double delta_t);

    /// Whether this scene stops the previous music even if wont play any music.
    void StopsPreviousMusic(bool set) { stops_previous_music_ = set; }

    /** @name Getters and Setters
    @{
    */
    bool finished() const { return finished_; }

          graphic::Node*   content_node()       { return   content_node_; }
    const graphic::Node*   content_node() const { return   content_node_; }
          graphic::Node* interface_node()       { return interface_node_; }
    const graphic::Node* interface_node() const { return interface_node_; }

          MediaManager& media_manager()       { return media_manager_; }
    const MediaManager& media_manager() const { return media_manager_; }

    audio::Music* background_music() const { return background_music_; }
    void set_background_music(audio::Music* music) { background_music_ = music; }
    /**@}
     */

    void set_defocus_callback(std::function<void (Scene*)> defocus_callback) { 
        defocus_callback_ = defocus_callback;
    }
    void set_focus_callback(std::function<void (Scene*)> focus_callback) { 
        focus_callback_ = focus_callback;
    }

  protected:
    
    /// Ends the scene activity.
    /** Note: do not release any resources in this method. */
    virtual void End();

    /// Tells whether scene is finished or not.
    bool finished_;

    /// The background music when this scene is on top.
    audio::Music* background_music_;

  private:
    /// Whether this scene stops the previous music even if wont play any music.
    bool stops_previous_music_;

    graphic::Node*   content_node_;
    graphic::Node* interface_node_;

    /// A MediaManager provided for users.
    MediaManager media_manager_;

    std::list<Entity*> entities_;
    std::queue<Entity*> queued_entities_;
    std::function<void (Scene*)> defocus_callback_;
    std::function<void (Scene*)> focus_callback_;

    struct OrderedTask {
        OrderedTask(int p, const Task& t) : priority(p), task(t) {}

        int priority;
        Task task;

        bool operator< (const OrderedTask& other) const { return priority < other.priority; }
        bool operator()(double dt) { return task(dt); }
    };
    std::list<OrderedTask> tasks_;
   
  friend class Engine;
}; // class Scene.

} /* namespace action */

} /* namespace ugdk */

#endif /* UGDK_ACTION_SCENE_H_ */
