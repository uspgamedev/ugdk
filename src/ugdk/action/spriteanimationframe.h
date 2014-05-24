#ifndef UGDK_ACTION_SPRITEANIMATIONFRAME_H_
#define UGDK_ACTION_SPRITEANIMATIONFRAME_H_

#include <ugdk/graphic/geometry.h>
#include <ugdk/graphic/visualeffect.h>

#include <string>

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
    SpriteAnimationFrame(const std::string& atlas_frame_name)
        : atlas_frame_name_(atlas_frame_name)
        , mirror_(0)
        , period_(DEFAULT_PERIOD)
    {}

    const std::string& atlas_frame_name() const { return atlas_frame_name_; }
    const graphic::Geometry& geometry() const { return geometry_; }
    const graphic::VisualEffect& effect() const { return effect_; }
    ugdk::Mirror mirror() const { return mirror_; }
    double fps() const { return 1.0/period_; }
    double period() const { return period_; }

    void set_atlas_frame_name(const std::string& name) { atlas_frame_name_ = name; }
    graphic::Geometry& geometry() { return geometry_; }
    graphic::VisualEffect& effect() { return effect_; }
    void set_mirror(const ugdk::Mirror& _mirror) { mirror_ = _mirror; }
    void set_fps(double _fps) { period_ = 1.0/_fps; }
    void set_period(double _period) { period_ = _period; }

    static const SpriteAnimationFrame& DEFAULT() {
        static SpriteAnimationFrame default_frame("default");
        return default_frame;
    }

  private:
    std::string atlas_frame_name_;
    graphic::Geometry geometry_;
    graphic::VisualEffect effect_;
    ugdk::Mirror mirror_;
    double period_;
};

} /* namespace action */

} /* namespace ugdk */

#endif /* UGDK_ACTION_ANIMATIONFRAME_H_ */


