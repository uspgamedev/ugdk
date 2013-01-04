#ifndef UGDK_ACTION_SPRITEANIMATIONFRAME_H_
#define UGDK_ACTION_SPRITEANIMATIONFRAME_H_

#include <vector>
#include <string>

#include <ugdk/action.h>
#include <ugdk/graphic.h>
#include <ugdk/graphic/geometry.h>
#include <ugdk/graphic/visualeffect.h>

#define DEFAULT_PERIOD 0.1

namespace ugdk {
namespace action {

/*
 * Represents the visual behavior information of a sprite in a single game frame.
 */
class SpriteAnimationFrame {
  /*
   * frame_: the index of the spritesheet frame that should be rendered.
   * geometry_: a pointer to the Geometry object describing the visual modifiers that
   *            should be applied to the rendered sprite.
   */
  public:
    SpriteAnimationFrame(int frame)
        : frame_(frame) {}

    int frame() const { return frame_; }
    const graphic::Geometry& geometry() const { return geometry_; }
    const graphic::VisualEffect& effect() const { return effect_; }
    ugdk::Mirror mirror() const { return mirror_; }

    void set_frame(const int frame) { frame_ = frame; }
    graphic::Geometry& geometry() { return geometry_; }
    graphic::VisualEffect& effect() { return effect_; }
    void set_mirror(const ugdk::Mirror& mirror) { mirror_ = mirror; }

  private:
    int frame_;
    graphic::Geometry geometry_;
    graphic::VisualEffect effect_;
    ugdk::Mirror mirror_;
};

/*
 * Is a complex of a vector with a sequence of frame indexes, and a fixed period/fps.
 */
class SpriteAnimation {
  public:
    typedef SpriteAnimationFrame Frame;
    SpriteAnimation() : period_(DEFAULT_PERIOD) {}
    
    /* try to use period() instead whenever you can */
    double fps() const { return 1.0/period_; }
    double period() const { return period_; }
    size_t size() const { return animation_frames_.size(); }

    /* try to use set_period() instead whenever you can */
    void set_fps(const double fps) { period_ = 1.0/fps; }
    void set_period(const double period) { period_ = period; }
   
    Frame* At(size_t i) const { return animation_frames_.at(i); }
    void Add(Frame* f) { animation_frames_.push_back(f); }

  private:
    std::vector<Frame*> animation_frames_;
    double period_;

};

} /* namespace action */

} /* namespace ugdk */

#endif /* UGDK_ACTION_ANIMATIONFRAME_H_ */


