#ifndef UGDK_STRUCTURE_VISUALEFFECT_H_
#define UGDK_STRUCTURE_VISUALEFFECT_H_

#include <ugdk/structure/color.h>

namespace ugdk {
namespace structure {

class VisualEffect {
  public:
    /// Creates an identity VisualEffect;
    VisualEffect() : visible_(true) {}

    ///Creates a new VisualEffect object with the specified values. 
    /**
     * @param color The color.
     */
    VisualEffect(const Color& color, bool _visible = true) :
        color_(color), visible_(_visible) {}

    ~VisualEffect() {}

    // Getters.
    /**@name Getters and Setters
     *@{
     */
    const Color& color() const { return color_; }
    bool visible() const { return visible_; }

    void set_color(const Color& _color) { color_ = _color; }
    void set_visible(bool _visible) { visible_ = _visible; }
    /**@}
     */

    void Compose(const VisualEffect& other) {
        color_.Compose(other.color_);
        visible_ = visible_ && other.visible_;
    }

    VisualEffect& operator *= (const VisualEffect& rhs) { 
        Compose(rhs);
        return *this;
    }
    
    VisualEffect operator * (const VisualEffect& rhs) const {
        VisualEffect result(*this);
        result.Compose(rhs);
        return result;
    }

  private:
    Color color_;
    bool visible_;
};

}  // namespace structure
}  // namespace ugdk

#endif // UGDK_STRUCTURE_VISUALEFFECT_H_
