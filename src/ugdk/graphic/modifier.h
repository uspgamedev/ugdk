#ifndef HORUSEYE_FRAMEWORK_MODIFIER_H_
#define HORUSEYE_FRAMEWORK_MODIFIER_H_

#include <vector>
#include <ugdk/math/vector2D.h>
#include <ugdk/base/types.h>

namespace ugdk {

class Modifier {

  public:

    /**
     * Creates a new Modifier object with the specified values.
     * The names of the parameters are self-explanatory.
     */
    Modifier(Vector2D offset = Vector2D(), Vector2D scale = Vector2D(1.0f,1.0f),
             float rotation = 0.0f, Mirror mirror = MIRROR_NONE, Color color = WHITE, float alpha = 1.0f) :
        offset_(offset), scale_(scale), rotation_(rotation), mirror_(mirror), color_(color), alpha_(alpha) {}

    // Copy constructor
    Modifier(Modifier &mod);

    // Destructor
    ~Modifier() {}

    // Getters.
    Vector2D offset()   const { return   offset_; }
    Vector2D scale()    const { return    scale_; }
    float    rotation() const { return rotation_; }
    Mirror   mirror()   const { return   mirror_; }
    Color    color()    const { return    color_; }
    float    alpha()    const { return    alpha_; }

    // Setters.
    void set_offset(const Vector2D& offset) { offset_ = offset; }
    void set_scale(const Vector2D& scale)   { scale_  = scale;  }
    /// Adjusts rotation to use the [0,2PI] space and sets it to the Modifier.
    void set_rotation(const float rotation);
    /// Assigns MIRROR_NONE in case of an invalid argument.
    void set_mirror(const Mirror mirror);
    /// Truncates each component to [0,1] and sets it to the Modifier.
    void set_color(const Color color);
    /// Truncates alpha to [0,1] and sets it to the Modifier.
    void set_alpha(const float alpha);

    // Component composers.
    void ComposeOffset(const Vector2D& offset) { offset_  += offset;  }
    void ComposeScale(const Vector2D& scale)   { scale_.x *= scale.x;
                                                 scale_.y *= scale.y; }
    /// Adjusts rotation to use the [0,2PI] space and composes on the Modifier.
    void ComposeRotation(const float rotation);
    /// Does nothing if mirror == MIRROR_NONE or if mirror is invalid.
    void ComposeMirror(const Mirror mirror);
    /// Truncates each component to [0,1] and composes on the Modifier.
    void ComposeColor(const Color& color);
    /// Truncates alpha to [0,1] and composes on the Modifier.
    void ComposeAlpha(const float alpha);

    void ComposeOffset(   const Modifier* mod2 ) { if(mod2 == NULL) return; ComposeOffset(   mod2->offset()   ); }
    void ComposeScale(    const Modifier* mod2 ) { if(mod2 == NULL) return; ComposeScale(    mod2->scale()    ); }
    void ComposeRotation( const Modifier* mod2 ) { if(mod2 == NULL) return; ComposeRotation( mod2->rotation() ); }
    void ComposeMirror(   const Modifier* mod2 ) { if(mod2 == NULL) return; ComposeMirror(   mod2->mirror()   ); }
    void ComposeColor(    const Modifier* mod2 ) { if(mod2 == NULL) return; ComposeColor(    mod2->color()    ); }
    void ComposeAlpha(    const Modifier* mod2 ) { if(mod2 == NULL) return; ComposeAlpha(    mod2->alpha()    ); }

    void Compose(const Modifier* mod2);

  private:

    Vector2D        offset_,
                    scale_;
    float           rotation_;
    Mirror          mirror_;
    Color           color_; //TODO: Modifier's "color_" is actually a light filter.
    float           alpha_;
    //TODO: colocar spreadsheet number no modifier.

};

}

#endif /* HORUSEYE_FRAMEWORK_MODIFIER_H_ */