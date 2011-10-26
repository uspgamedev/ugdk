#ifndef HORUSEYE_FRAMEWORK_MODIFIER_H_
#define HORUSEYE_FRAMEWORK_MODIFIER_H_

#include <vector>
#include <ugdk/math/vector2D.h>
#include <ugdk/base/types.h>

namespace ugdk {

class Modifier {

  public:

    /**
     * Creates a new Modifier object with the specified values. The names of
     * the parameters are self explanatories.
     */
    Modifier(Vector2D offset = Vector2D(), Vector2D size = Vector2D(1.0f,1.0f),
              float rotation = 0.0f, Mirror mirror = MIRROR_NONE,
              Color color = WHITE, float alpha = 1.0) :
                  offset_(offset), scale_(size), rotation_(rotation),
                  mirror_(mirror), color_(color), alpha_(alpha) {}

    // Copy constructor
    Modifier(Modifier &mod);

    // Destructor
    ~Modifier() {}

    // Getters.
    Vector2D offset()   const { return   offset_; }
    Vector2D scale()     const { return     scale_; }
    float    rotation() const { return rotation_; }
    Mirror   mirror()   const { return   mirror_; }
    Color    color()    const { return    color_; }
    float    alpha()    const { return    alpha_; }

    // Setters.
    void set_offset(const Vector2D& offset) { offset_   = offset;   }
    void set_scale(const Vector2D& size)     { scale_     = size;     }
    void set_rotation(const float rotation) { rotation_ = rotation; }
    void set_mirror(const Mirror mirror)    { mirror_   = mirror;   }
    void set_color(const Color color)       { color_    = color;    }
    void set_alpha(const float alpha)       { alpha_    = alpha;    }

    // Component composers.
    void ComposeOffset(const Vector2D& offset) { offset_    += offset;   }
    void ComposeScale(const Vector2D& size)     { scale_.x    *= size.x;
                                                 scale_.y    *= size.y;   }
    void ComposeRotation(const float rotation) { rotation_  *= rotation; }
    void ComposeMirror(const Mirror mirror)    { mirror_    ^= mirror;   }
    void ComposeColor(const Color& color)      { color_.r   *= color.r;
                                                 color_.g   *= color.g;
                                                 color_.b   *= color.b;  }
    void ComposeAlpha(const float alpha)       { alpha_     *= alpha;    }

    void ComposeOffset(const Modifier *mod2)   { ComposeOffset(   mod2->offset()   ); }
    void ComposeScale(const Modifier *mod2)     { ComposeScale(     mod2->scale()     ); }
    void ComposeRotation(const Modifier *mod2) { ComposeRotation( mod2->rotation() ); }
    void ComposeMirror(const Modifier *mod2)   { ComposeMirror(   mod2->mirror()   ); }
    void ComposeColor(const Modifier *mod2)    { ComposeColor(    mod2->color()    ); }
    void ComposeAlpha(const Modifier *mod2)    { ComposeAlpha(    mod2->alpha()    ); }

    void Compose(const Modifier *mod2);

  private:

    Vector2D        offset_,
                    scale_;
    float           rotation_;
    Mirror          mirror_;
    Color           color_;
    float           alpha_;
    //TODO: colocar spreadsheet number no modifier.

};

}

#endif /* HORUSEYE_FRAMEWORK_MODIFIER_H_ */
