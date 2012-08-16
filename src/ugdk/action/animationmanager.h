#ifndef UGDK_ACTION_ANIMATIONMANAGER_H_
#define UGDK_ACTION_ANIMATIONMANAGER_H_

#include <vector>
#include <string>
#include <ugdk/portable/tr1.h>
#include FROM_TR1(functional)

#include <ugdk/action.h>
#include <ugdk/graphic.h>
#include <ugdk/action/animationframe.h> // FIXME: not necessary.

#define DEFAULT_PERIOD 0.1

namespace ugdk {
namespace action {

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
    AnimationManager(AnimationSet *set);
    ~AnimationManager();

    void set_slowdown_factor(const double factor) { period_scaling_factor_ = factor; }
    //Note: try to use set_slowdown_factor() instead whenever you can.
    void set_speedup_factor(const double factor) { set_slowdown_factor(1.0/factor); }

    //Note: try to use period() instead whenever you can.
    double    fps() const;
    double period() const;
    unsigned int n_frames() const;

    int GetFrame() const;
    void set_default_frame(int default_frame) { default_frame_ = default_frame; }
    const graphic::Modifier* get_current_modifier() const;
    void Select(const std::string& name);
    void Select(int index);
    void Update(double delta_t);
    void AddObserver(Observer* observer);
    void AddTickFunction(std::tr1::function<void (void)> tick);

  private:
    double period_scaling_factor_;

    Animation *current_animation_;
    AnimationSet *animation_set_;
    int current_frame_;
    int default_frame_;
    double elapsed_time_;

    std::vector<Observer *> observers_;
    std::vector< std::tr1::function<void (void)> > ticks_;
    void NotifyAllObservers();

};

} /* namespace action */

} /* namespace ugdk */

#endif /* UGDK_ACTION_ANIMATION_H_ */


