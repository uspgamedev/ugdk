#ifndef HORUSEYE_FRAMEWORK_SPRITE_H_
#define HORUSEYE_FRAMEWORK_SPRITE_H_

#include <ugdk/math/vector2D.h>
#include <ugdk/graphic/drawable.h>

#include <ugdk/action/animation.h>


namespace ugdk {

class AnimationSet;
class Spritesheet;

class Sprite : public Drawable {
  public:
    Sprite(Spritesheet *spritesheet, AnimationSet *set = NULL);
    virtual ~Sprite();

    void Draw();
    
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
    *  @param factor is a float 
    */
    void SetAnimationSpeedupFactor(float factor)  { animation_manager_->set_speedup_factor(factor);  }

    /// Change the SlowdownFactor of the animation
    /** Given a a factor, this function set the animation SlowdownFactor to this value
    *  @param factor is a float 
    */
    void SetAnimationSlowdownFactor(float factor) { animation_manager_->set_slowdown_factor(factor); }

    /// Return the animation FPS
    /** Return the animation FPS
    *  @return	a float equivalente to the animation FPS
    */
    float GetAnimationFPS() const { return animation_manager_->fps(); }

    /// Add a observer object to the animation
    /** Given an observer object, the function include this in the animation manager
    *  @param *observer is a pointer to the observer object
    */
    void AddObserverToAnimation(Observer *observer) {
        animation_manager_->AddObserver(observer);
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

    /// Update the Sprite based on the time variation.
    /** One of the two main functions of the UGDK Engine. Most of the game logic 
        resides within the Update of child classes.*/
    virtual void Update(float delta_t);
    
  private:
    Spritesheet *spritesheet_;
    AnimationManager *animation_manager_;
};

}

#endif //HORUSEYE_FRAMEWORK_SPRITE_H_
