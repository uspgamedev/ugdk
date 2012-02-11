#ifndef HORUSEYE_FRAMEWORK_ANIMATION_H_
#define HORUSEYE_FRAMEWORK_ANIMATION_H_

#include <vector>
#include <string>
#include <ugdk/graphic/modifier.h>

#define DEFAULT_PERIOD 0.1

namespace ugdk {

class Observer;
class AnimationSet;

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

    /*
     * Represents the visual behavior information of a sprite in a single game frame.
     */
    class AnimationFrame {
      /*
       * frame_: the index of the spritesheet frame that should be rendered.
       * modifier_: a pointer to the Modifier object describing the visual modifiers that
       *            should be applied to the rendered sprite.
       */
      public:
        AnimationFrame(int frame, graphic::Modifier *modifier = NULL)
            : frame_(frame), modifier_(modifier) {}

        int frame() const { return frame_; }
        graphic::Modifier *modifier() const { return modifier_; }

        void set_frame(const int frame) { frame_ = frame; }
      private:
        int frame_;
        graphic::Modifier *modifier_;
    };

    /*
     * Is a complex of a vector with a sequence of frame indexes, and a fixed period/fps.
     */
    class Animation : public std::vector<AnimationFrame*> {
      /*
       * period_: the inverse of the animation's fps.
       */
      public:
        Animation() : std::vector<AnimationFrame*>(), period_(DEFAULT_PERIOD) {}

        /* try to use period() instead whenever you can */
        double fps() const { return 1.0/period_; }
        double period() const { return period_; }

        /* try to use set_period() instead whenever you can */
        void set_fps(const double fps) { period_ = 1.0/fps; }
        void set_period(const double period) { period_ = period; }

      private:
        double period_;

    };


    AnimationManager(double fps, AnimationSet *set);/*TODO: remove fps*/
    ~AnimationManager();

    void set_slowdown_factor(const double factor) { period_scaling_factor_ = factor; }
    //Note: try to use set_slowdown_factor() instead whenever you can.
    void set_speedup_factor(const double factor) { set_slowdown_factor(1.0/factor); }

    //Note: try to use period() instead whenever you can.
    double    fps() const { return 1.0/(period_scaling_factor_*current_animation_->period()); }
    double period() const { return period_scaling_factor_*current_animation_->period(); }
    int n_frames() const { return current_animation_->size(); }

    int GetFrame();
    void set_default_frame(int default_frame) {
        default_frame_ = default_frame;
    }
    const graphic::Modifier* get_current_modifier() const {
        return current_animation_
                ? current_animation_->at(current_frame_)->modifier()
                : NULL;
    }
    void Select(std::string name);
    void Select(int index);
    void Update(double delta_t);
    void AddObserver(Observer* observer);

  private:
    double period_scaling_factor_;

    Animation *current_animation_;
    AnimationSet *animation_set_;
    int current_frame_;
    int default_frame_;
    double elapsed_time_;

    std::vector<Observer *> observers;
    void NotifyAllObservers();

};

}
#endif


