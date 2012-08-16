#ifndef UGDK_ACTION_ANIMATIONMANAGER_H_
#define UGDK_ACTION_ANIMATIONMANAGER_H_

#include <vector>
#include <string>
#include <ugdk/portable/tr1.h>
#include FROM_TR1(functional)

#include <ugdk/action.h>
#include <ugdk/graphic.h>

#define DEFAULT_PERIOD 0.1

namespace ugdk {
namespace action {

class Animation {
public:
    virtual double period() const = 0;
    virtual size_t size() const = 0;
protected:
    Animation() {}
};

/*TODO
 * Represents a sprite's current animation.
 *
 * An AnimationManager object contains a sequence of information describing a sprite's visual behavior
 * (animation spreadsheet frames, position, rotation, transparency, etc) throughout a corresponding sequence
 * of the game's update frames. This per-frame information is represented by the class
 * AnimationManager::AnimationFrame, and the sequence of this information by the AnimationManager::Animation
 * type.
 *
 * Also, an AnimationManager object contains a set of all the possible animations the sprite may need.
 * This set is represented by the AnimationSet class. Thus, a sprite needs but one AnimationManager object
 * to access any of the animations it requires, as long as these are all properly registered
 * in the AnimationSet object given to the AnimationManager object.
 */
class AnimationManager {
  public:
    AnimationManager();
    virtual ~AnimationManager();

    void set_current_animation(const Animation* animation);
    const Animation* current_animation() const { return current_animation_; }
    int current_animation_frame_index() const { return current_frame_; }

    void Update(double dt);

    /// Add a observer object to the animation
    /** Given an observer object, the function include this in the animation manager
    *  @param *observer is a pointer to the observer object
    */
    void AddObserver(Observer* observer);

    void AddTickFunction(std::tr1::function<void (void)> tick);

  private:
    const Animation *current_animation_;
    int current_frame_;
    double elapsed_time_;

    std::vector<Observer *> observers_;
    std::vector< std::tr1::function<void (void)> > ticks_;
    void notifyAllObservers();
};

} /* namespace action */
} /* namespace ugdk */

#endif /* UGDK_ACTION_ANIMATIONMANAGER_H_ */


