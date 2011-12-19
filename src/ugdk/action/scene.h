#ifndef HORUSEYE_FRAMEWORK_SCENE_H_
#define HORUSEYE_FRAMEWORK_SCENE_H_

#include <ugdk/action/layer.h>
#include <ugdk/graphic/node.h>
#include <list>

namespace ugdk {

class Music;

/**
   @class Scene
   @brief A game scene, or screen.

   Represents a scene, containing various graphical layers and sprites.
   A scene is game screen, such as a menu, or the playable part
   itself.
*/
class Scene {
  public:
    Scene() : finished_(false), background_music_(NULL), stops_previous_music_(true), root_node_(new Node) {}
  /**
     Note: Destroys all layers in the scene.
  */
    virtual ~Scene();

    /// Method called when this Scene arrives on the top of the Scene stack.
    virtual void Focus();

    /// Method called when this Scene leaves the top of the Scene stack.
    virtual void DeFocus() {}

    /// Adds a Layer to the scene on top of current layers.
    void AddLayer(Layer *layer) { layers_.push_back(layer); };
    /// Removes the specified Layer from the scene.
    void RemoveLayer(Layer *layer) { layers_.remove(layer); };

    /// Finishes the scene.
    void Finish() { End(); finished_ = true; }

    /// Logical update of the scene.
    /**
       @param delta_t Time in seconds since last update
    */
    virtual void Update(float delta_t);

    /// Redraws the scene's graphics.
    void Render();

    /// Redraws the lightsources.
    void RenderLight();

    /// Whether this scene stops the previous music even if wont play any music.
    void StopsPreviousMusic(bool set) { stops_previous_music_ = set; }

    /** @name Getters and Setters
    @{
    */
    bool finished() const { return finished_; }

    Node* root_node() { return root_node_; }
    const Node* root_node() const { return root_node_; }

    Music* background_music() const { return background_music_; }
    void set_background_music(Music* music) { background_music_ = music; }
    
    bool visible() const { return root_node_->visible(); }
    void set_visible(bool set) { root_node_->set_visible(set); }
    /**@}
     */

  protected:

    /// Ends the scene activity.
    /** Note: do not release any resources in this method. */
    virtual void End();

    /// Layer list
    ::std::list<Layer*> layers_;

    /// Tells whether scene is finished or not.
    bool finished_;
    bool visible_;

    /// The background music when this scene is on top.
    Music* background_music_;

  private:
    /// Whether this scene stops the previous music even if wont play any music.
    bool stops_previous_music_;

    Node* root_node_;
   
  friend class Engine;
}; // class Scene.

}

#endif /* HORUSEYE_FRAMEWORK_SCENE_H_ */
