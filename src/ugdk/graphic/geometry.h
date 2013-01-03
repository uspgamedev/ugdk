#ifndef UGDK_GRAPHIC_GEOMETRY_H_
#define UGDK_GRAPHIC_GEOMETRY_H_

#include <ugdk/math/vector2D.h>

namespace ugdk {
namespace graphic {

class Geometry {
  public:
    /// Creates an identity Geometry;
    Geometry();

    ///Creates a new Geometry object with the specified values. 
    /**
     * @param offset The offset of the image.
     * @param size The size modifiers for the image. X and Y values can be set
     * independently.
     * @param rotation Rotation angle in radians. 0 points to the right, increases in
     * counterclockwise fashion.
     * @param mirror Mirrors the image. Can be MIRROR_NONE for no mirroring, 
     * MIRROR_HFLIP for horizontal mirroring or MIRROR_VFLIP for vetical mirroring.
     */
    Geometry(const math::Vector2D& _offset, const math::Vector2D _scale = math::Vector2D(1.0, 1.0), double _rotation = 0.0) :
        offset_(_offset), scale_(_scale), rotation_(_rotation) {}

    // Destructor
    ~Geometry() {}

    // Getters.
    /**@name Getters and Setters
     *@{
     */
    const ugdk::math::Vector2D& offset()    const { return   offset_; }
    const ugdk::math::Vector2D& scale()     const { return    scale_; }
    double          rotation()  const { return rotation_; }

    // Setters.
    void set_offset(const math::Vector2D& _offset) { offset_ = _offset; }
    void set_scale(const math::Vector2D& _scale)   { scale_  = _scale;  }
    void set_rotation(double rotation) { rotation_ = rotation; }
    /**@}
     */

    void Compose(const Geometry&);

    Geometry& operator *= (const Geometry& rhs) {
        Compose(rhs);
        return *this;
    }
    
    Geometry operator * (const Geometry& rhs) const {
        Geometry result(*this);
        result.Compose(rhs);
        return result;
    }

    void AsMatrix4x4(double M[16]) const;

  private:
    // Attributes
    math::Vector2D offset_, scale_;
    double   rotation_;
};

}  // namespace graphic
}  // namespace ugdk

#endif /* UGDK_GRAPHIC_MODIFIER_H_ */
