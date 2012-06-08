#ifndef HORUSEYE_FRAMEWORK_SPRITE_H_
#define HORUSEYE_FRAMEWORK_SPRITE_H_

#include <ugdk/math/vector2D.h>
#include <ugdk/graphic.h>
#include <ugdk/graphic/drawable.h>
#include <ugdk/action/animation.h>

namespace ugdk {
namespace graphic {

class Sprite : public Drawable {
  public:
    Sprite(Spritesheet *spritesheet, action::AnimationSet *set = NULL);
    virtual ~Sprite();

    void Draw(double dt);
    const Vector2D& size() const;
    
    /// Change the current animation to a new animation from the previously selected AnimationSet.
    /**Given a animation name (a string), the function changes the current animation to a new animation of AnimationSet*/
    void SelectAnimation(std::string animation_name) {
        animation_manager_->Select(animation_name);
    }
    /// Change the current animation to a new animation from the previously selected AnimationSet.
    /**Given a animation index (a integer), the function changes the current animation to a new animation of AnimationSet*/
    void SelectAnimation(int animation_index) {
        animation_manager_->Select(animation_index);
    }

    /// Change the SpeedupFactor of the animation
    /** Given a a factor, this function set the animation SpeedupFactor to this value
    *  @param factor is a double 
    */
    void SetAnimationSpeedupFactor(double factor)  { animation_manager_->set_speedup_factor(factor);  }

    /// Change the SlowdownFactor of the animation
    /** Given a a factor, this function set the animation SlowdownFactor to this value
    *  @param factor is a double 
    */
    void SetAnimationSlowdownFactor(double factor) { animation_manager_->set_slowdown_factor(factor); }

    /// Return the animation FPS
    /** Return the animation FPS
    *  @return	a double equivalente to the animation FPS
    */
    double GetAnimationFPS() const { return animation_manager_->fps(); }

    /// Add a observer object to the animation
    /** Given an observer object, the function include this in the animation manager
    *  @param *observer is a pointer to the observer object
    */
    void AddObserverToAnimation(action::Observer *observer) {
        animation_manager_->AddObserver(observer);
    }
    
    void AddTickFunctionToAnimation(std::tr1::function<void (void)> tick) {
        animation_manager_->AddTickFunction(tick);
    }

    /// Return the animation frame number
    /** @return an integer that is the animation frame number
    */
    int GetAnimationFrameNumber() const { return animation_manager_->n_frames(); }
    
    /// Set the default frame
    /** @param frame is a integer
    */
    void SetDefaultFrame(int frame) {
        animation_manager_->set_default_frame(frame);
    }

       
  private:
    Spritesheet *spritesheet_;
    action::AnimationManager *animation_manager_;

    /// Update the Sprite based on the time variation.
    /** One of the two main functions of the UGDK Engine. Most of the game logic 
        resides within the Update of child classes.*/
    void Update(double delta_t);
};

}  // namespace graphic
}  // namespace ugdk

#endif //HORUSEYE_FRAMEWORK_SPRITE_H_
