#ifndef UGDK_ACTION_SPRITEANIMATIONFRAME_H_
#define UGDK_ACTION_SPRITEANIMATIONFRAME_H_

#include <vector>
#include <string>

#include <ugdk/action/animationmanager.h>

#include <ugdk/action.h>
#include <ugdk/graphic.h>

#define DEFAULT_PERIOD 0.1

namespace ugdk {
namespace action {

/*
 * Represents the visual behavior information of a sprite in a single game frame.
 */
class SpriteAnimationFrame {
  /*
   * frame_: the index of the spritesheet frame that should be rendered.
   * modifier_: a pointer to the Modifier object describing the visual modifiers that
   *            should be applied to the rendered sprite.
   */
  public:
    SpriteAnimationFrame(int frame, graphic::Modifier *modifier = NULL)
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
class SpriteAnimation : public virtual Animation {
  /*
   * period_: the inverse of the animation's fps.
   */
  public:
    typedef SpriteAnimationFrame Frame;
    SpriteAnimation() : period_(DEFAULT_PERIOD) {}

    /* try to use period() instead whenever you can */
    double fps() const { return 1.0/period_; }
    double period() const { return period_; }
    size_t size() const { return animation_frames_.size(); }
    Frame* at(size_t i) const { return animation_frames_.at(i); }
    void push_back(Frame* f) { animation_frames_.push_back(f); }

    /* try to use set_period() instead whenever you can */
    void set_fps(const double fps) { period_ = 1.0/fps; }
    void set_period(const double period) { period_ = period; }

  private:
    std::vector<Frame*> animation_frames_;
    double period_;

};

} /* namespace action */

} /* namespace ugdk */

#endif /* UGDK_ACTION_ANIMATIONFRAME_H_ */


