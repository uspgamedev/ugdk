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
    Modifier(const Vector2D& offset, const Vector2D scale = Vector2D(1.0, 1.0),
             double rotation = 0.0, Mirror mirror = MIRROR_NONE, const Color color = WHITE, const bool visible = true) :
        
        offset_(offset), scale_(scale), rotation_(rotation), mirror_(mirror), 
        color_(color), visible_(visible), flags_(HAS_TRANSFORMATION | HAS_COLOR) {}

    // Destructor
    ~Modifier() {}

    // Getters.
    /**@name Getters and Setters
     *@{
     */
    const Vector2D& offset()    const { return   offset_; }
    const Vector2D& scale()     const { return    scale_; }
    double          rotation()  const { return rotation_; }
    const Mirror&   mirror()    const { return   mirror_; }
    const Color&    color()     const { return    color_; }
    double          alpha()     const { return  color_.a; }
    uint16          flags()     const { return    flags_; }
    bool            visible()   const { return  visible_; }

    // Setters.
    void set_offset(const Vector2D& offset) { offset_ = offset; flags_ |= HAS_TRANSFORMATION; }
    void set_scale(const Vector2D& scale)   { scale_  = scale;  flags_ |= HAS_TRANSFORMATION; }
    /// Adjusts rotation to use the [0,2PI] space and sets it to the Modifier.
    void set_rotation(const double rotation);
    /// Assigns MIRROR_NONE in case of an invalid argument.
    void set_mirror(const Mirror mirror);
    /// Truncates each component to [0,1] and sets it to the Modifier.
    void set_color(const Color& color);
    /// Truncates alpha to [0,1] and sets it to the Modifier.
    void set_alpha(const double alpha);
    void set_visible(const bool visible) { visible_ = visible; }
    /**@}
     */
    /**@name Component composers.
     *@{
     */
    void ComposeOffset(const Vector2D& offset) { offset_  += offset;  flags_ |= HAS_TRANSFORMATION;}
    void ComposeScale(const Vector2D& scale)   { scale_.x *= scale.x;
                                                 scale_.y *= scale.y; 
                                                 flags_ |= HAS_TRANSFORMATION; }

    void ToggleFlag(uint16 flag) { flags_ ^= flag; }
    /// Adjusts rotation to use the [0,2PI] space and composes on the Modifier.
    void ComposeRotation(const double rotation);
    /// Does nothing if mirror == MIRROR_NONE or if mirror is invalid.
    void ComposeMirror(const Mirror& mirror);
    /// Truncates each component to [0,1] and composes on the Modifier.
    void ComposeColor(const Color& color);
    /// Truncates alpha to [0,1] and composes on the Modifier.    void ComposeAlpha(const double alpha);
	void ComposeAlpha(const double alpha);
    void ComposeVisible(const bool visible) { visible_ = !(!visible_ || !visible); }
    
    void ComposeOffset(   const Modifier* mod2 ) { if(mod2 == NULL) return; ComposeOffset(   mod2->offset_   ); }
    void ComposeScale(    const Modifier* mod2 ) { if(mod2 == NULL) return; ComposeScale(    mod2->scale_    ); }
    void ComposeRotation( const Modifier* mod2 ) { if(mod2 == NULL) return; ComposeRotation( mod2->rotation_ ); }
    void ComposeMirror(   const Modifier* mod2 ) { if(mod2 == NULL) return; ComposeMirror(   mod2->mirror_   ); }
    void ComposeColor(    const Modifier* mod2 ) { if(mod2 == NULL) return; ComposeColor(    mod2->color_    ); }
    void ComposeVisible(  const Modifier* mod2 ) { if(mod2 == NULL) return; ComposeVisible(  mod2->visible_  ); }

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
    Vector2D        offset_,
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
