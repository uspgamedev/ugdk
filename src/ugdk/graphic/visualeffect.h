#ifndef UGDK_GRAPHIC_VISUALEFFECT_H_
#define UGDK_GRAPHIC_VISUALEFFECT_H_

#include <ugdk/base/types.h>

namespace ugdk {
namespace graphic {

class VisualEffect {
  public:
    /// Creates an identity VisualEffect;
    VisualEffect() {}

    ///Creates a new VisualEffect object with the specified values. 
    /**
     * @param color The color.
     */
    VisualEffect(const ugdk::Color& _color) :
        color_(_color) {}

    ~VisualEffect() {}

    // Getters.
    /**@name Getters and Setters
     *@{
     */
    const ugdk::Color& color() const { return color_; }

    void set_color(const ugdk::Color& _color) { color_ = _color; }
    /**@}
     */

    void Compose(const VisualEffect& other) {
        color_.Compose(other.color_);
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
    ugdk::Color color_;
};

}  // namespace graphic
}  // namespace ugdk

#endif // UGDK_GRAPHIC_VISUALEFFECT_H_
