#ifndef UGDK_ACTION_SCENE_H_
#define UGDK_ACTION_SCENE_H_

#include <ugdk/action/mediamanager.h>
#include <ugdk/structure/types.h>
#include <ugdk/system/taskplayer.h>
#include <ugdk/system/eventhandler.h>

#include <ugdk/action.h>
#include <ugdk/audio.h>
#include <ugdk/graphic.h>

#include <functional>
#include <list>
#include <queue>
#include <string>

namespace ugdk {
namespace action {

/**
   @class Scene
   @brief A game scene, or screen.

   Represents a scene, containing various graphical layers and sprites.
   A scene is game screen, such as a menu, or the playable part
   itself.
*/
class Scene : public system::TaskPlayer {
  public:
    Scene();
    virtual ~Scene();

    /// Method called when this Scene arrives on the top of the Scene stack.
    virtual void Focus();

    /// Method called when this Scene leaves the top of the Scene stack.
    virtual void DeFocus();

    /// Method called when this scene is pushed to the Engine's Scene stack.
    virtual void OnPushed(int index) {}

    /// Finishes the scene.
    void Finish();

    /// Whether this scene stops the previous music even if wont play any music.
    void StopsPreviousMusic(bool set) { stops_previous_music_ = set; }

    /// Logical update of the scene.
    /**
       @param delta_t Time in seconds since last update
    */
    void Update(double delta_t);

    /// Renders the scene to the given canvas if the scene is visible.
    /**
        Uses the configurated renderfunction.
    */
    void Render(uint32_t, graphic::Canvas&) const;

    /** @name Getters and Setters
    @{
    */
    const std::string& identifier() const { return identifier_; }
    void set_identifier(const std::string& new_identifier) { identifier_ = new_identifier; }

    bool finished() const { return finished_; }

    bool active() const { return active_; }
    void set_active(bool is_active) { active_ = is_active; }

    bool visible() const { return visible_; }
    void set_visible(bool is_visible) { visible_ = is_visible; }

          MediaManager& media_manager()       { return media_manager_; }
    const MediaManager& media_manager() const { return media_manager_; }

          system::EventHandler& event_handler()       { return event_handler_; }
    const system::EventHandler& event_handler() const { return event_handler_; }

    std::shared_ptr<audio::Music> background_music() const { return background_music_; }
    void set_background_music(std::shared_ptr<audio::Music> music) { background_music_ = music; }
    /**@}
     */

  protected:
    /// Ends the scene activity.
    /** Note: do not release any resources in this method. */
    virtual void End();

  private:
    /// Used to identify the scene internally.
    std::string identifier_;

    /// Tells whether the scene is currently running or not.
    bool active_;

    /// Tells whether the scene will be cleared on the next frame start.
    bool finished_;

    /// Tells whether the scene is visible or not.
    bool visible_;

    /// The background music when this scene is on top.
    std::shared_ptr<audio::Music> background_music_;

    /// Whether this scene stops the previous music even if wont play any music.
    bool stops_previous_music_;

    /// A MediaManager provided for users.
    MediaManager media_manager_;

    /// An scoped event handler for the scene.
    system::EventHandler event_handler_;

    /// Function that is used to render the scene when it's visible.
    std::vector< std::function<void (graphic::Canvas& canvases)> > render_functions_;
}; // class Scene.

} /* namespace action */
} /* namespace ugdk */

#endif /* UGDK_ACTION_SCENE_H_ */
