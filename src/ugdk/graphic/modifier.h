#ifndef UGDK_GRAPHIC_MODIFIER_H_
#define UGDK_GRAPHIC_MODIFIER_H_

#include <vector>
#include <cstddef>
#include <ugdk/math/vector2D.h>
#include <ugdk/base/types.h>

namespace ugdk {
namespace graphic {

class Modifier {
  public:
    enum Flags {
        NOTHING                 =  0,
        HAS_TRANSFORMATION      =  1,
        HAS_COLOR               =  2,
        TRUNCATES_WHEN_APPLIED  = 16
    };

    /// Creates an identity Modifier;
    Modifier();

    ///Creates a new Modifier object with the specified values. 
    /**
     * @param offset The offset of the image.
     * @param size The size modifiers for the image. X and Y values can be set
     * independently.
     * @param rotation Rotation angle in radians. 0 points to the right, increases in
     * counterclockwise fashion.
     * @param mirror Mirrors the image. Can be MIRROR_NONE for no mirroring, 
     * MIRROR_HFLIP for horizontal mirroring or MIRROR_VFLIP for vetical mirroring.
     * @param color The color filter.
     * @param alpha The alpha value for the image.
     */
    Modifier(const ugdk::math::Vector2D& _offset, const ugdk::math::Vector2D _scale = ugdk::math::Vector2D(1.0, 1.0),
             double _rotation = 0.0, Mirror _mirror = MIRROR_NONE, const Color _color = WHITE, bool _visible = true) :
        
        offset_(_offset), scale_(_scale), rotation_(_rotation), mirror_(_mirror), 
        color_(_color), visible_(_visible), flags_(HAS_TRANSFORMATION | HAS_COLOR) {}

    // Destructor
    ~Modifier() {}

    // Getters.
    /**@name Getters and Setters
     *@{
     */
    const ugdk::math::Vector2D& offset()    const { return   offset_; }
    const ugdk::math::Vector2D& scale()     const { return    scale_; }
    double          rotation()  const { return rotation_; }
    const Mirror&   mirror()    const { return   mirror_; }
    const Color&    color()     const { return    color_; }
    uint16          flags()     const { return    flags_; }
    bool            visible()   const { return  visible_; }

    // Setters.
    void set_offset(const ugdk::math::Vector2D& _offset) { offset_ = _offset; flags_ |= HAS_TRANSFORMATION; }
    void set_scale(const ugdk::math::Vector2D& _scale)   { scale_  = _scale;  flags_ |= HAS_TRANSFORMATION; }
    /// Adjusts rotation to use the [0,2PI] space and sets it to the Modifier.
    void set_rotation(const double rotation);
    /// Assigns MIRROR_NONE in case of an invalid argument.
    void set_mirror(const Mirror mirror);
    /// Truncates each component to [0,1] and sets it to the Modifier.
    void set_color(const Color& color);
    void set_visible(const bool _visible) { visible_ = _visible; }
    /**@}
     */
    /**@name Component composers.
     *@{
     */
    void ComposeOffset(const ugdk::math::Vector2D& _offset) { offset_  += _offset;  flags_ |= HAS_TRANSFORMATION;}
    void ComposeScale(const ugdk::math::Vector2D& _scale)   { scale_.x *= _scale.x;
                                                  scale_.y *= _scale.y; 
                                                  flags_ |= HAS_TRANSFORMATION; }

    void ToggleFlag(uint16 flag) { flags_ ^= flag; }
    /// Adjusts rotation to use the [0,2PI] space and composes on the Modifier.
    void ComposeRotation(const double rotation);
    /// Does nothing if mirror == MIRROR_NONE or if mirror is invalid.
    void ComposeMirror(const Mirror& mirror);
    /// Truncates each component to [0,1] and composes on the Modifier.
    void ComposeColor(const Color& color);
    void ComposeVisible(bool _visible) { visible_ = visible_ && _visible; }
    
    void ComposeOffset(   const Modifier* mod2 ) { if(mod2 == nullptr) return; ComposeOffset(   mod2->offset_   ); }
    void ComposeScale(    const Modifier* mod2 ) { if(mod2 == nullptr) return; ComposeScale(    mod2->scale_    ); }
    void ComposeRotation( const Modifier* mod2 ) { if(mod2 == nullptr) return; ComposeRotation( mod2->rotation_ ); }
    void ComposeMirror(   const Modifier* mod2 ) { if(mod2 == nullptr) return; ComposeMirror(   mod2->mirror_   ); }
    void ComposeColor(    const Modifier* mod2 ) { if(mod2 == nullptr) return; ComposeColor(    mod2->color_    ); }
    void ComposeVisible(  const Modifier* mod2 ) { if(mod2 == nullptr) return; ComposeVisible(  mod2->visible_  ); }

    /// Global composer and copy from pointer function.
    void Compose(const Modifier* mod2);
    /// Remember to free your new Modifier created through this static function!
    static Modifier* Compose(const Modifier* mod1, const Modifier* mod2);
    /**@}
     */
    /// Remember to free your new Modifier created through this static function!
    static Modifier* Copy(const Modifier* mod2);

  private:
    // Attributes
    ugdk::math::Vector2D        offset_,
                    scale_;
    double          rotation_;
    Mirror          mirror_;
    Color           color_; //TODO: Modifier's "color_" is actually a light filter.
    bool            visible_;
    uint16          flags_;
};

}  // namespace graphic
}  // namespace ugdk

#endif /* UGDK_GRAPHIC_MODIFIER_H_ */
