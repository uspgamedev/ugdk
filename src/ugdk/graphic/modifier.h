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
                  offset_(offset), size_(size), rotation_(rotation),
                  mirror_(mirror), color_(color), alpha_(alpha) {}

    // Copy constructor
    Modifier(Modifier &mod);

    // Destructor
    ~Modifier() {}

    // Getters.
    Vector2D offset()   const { return   offset_; }
    Vector2D size()     const { return     size_; }
    float    rotation() const { return rotation_; }
    Mirror   mirror()   const { return   mirror_; }
    Color    color()    const { return    color_; }
    float    alpha()    const { return    alpha_; }

    // Setters.
    void set_offset(const Vector2D& offset) { offset_   = offset;   }
    void set_size(const Vector2D& size)     { size_     = size;     }
    void set_rotation(const float rotation) { rotation_ = rotation; }
    void set_mirror(const Mirror mirror)    { mirror_   = mirror;   }
    void set_color(const Color color)       { color_    = color;    }
    void set_alpha(const float alpha)       { alpha_    = alpha;    }

    // Component composers.
    void ComposeOffset(const Vector2D& offset) { offset_    += offset;   }
    void ComposeSize(const Vector2D& size)     { size_.x    *= size.x;
                                                 size_.y    *= size.y;   }
    void ComposeRotation(const float rotation) { rotation_  *= rotation; }
    void ComposeMirror(const Mirror mirror)    { mirror_    ^= mirror;   }
    void ComposeColor(const Color& color)      { color_.r   *= color.r;
                                                 color_.g   *= color.g;
                                                 color_.b   *= color.b;  }
    void ComposeAlpha(const float alpha)       { alpha_     *= alpha;    }

    void ComposeOffset(const Modifier *mod2)   { ComposeOffset(   mod2->offset()   ); }
    void ComposeSize(const Modifier *mod2)     { ComposeSize(     mod2->size()     ); }
    void ComposeRotation(const Modifier *mod2) { ComposeRotation( mod2->rotation() ); }
    void ComposeMirror(const Modifier *mod2)   { ComposeMirror(   mod2->mirror()   ); }
    void ComposeColor(const Modifier *mod2)    { ComposeColor(    mod2->color()    ); }
    void ComposeAlpha(const Modifier *mod2)    { ComposeAlpha(    mod2->alpha()    ); }

    void Compose(const Modifier *mod2);

  private:

    Vector2D        offset_,
                    size_; //TODO: SIZE? ISSO AQUI … UM SCALE, POARR “w”
    float           rotation_;
    Mirror          mirror_;
    Color           color_;
    float           alpha_;
    //TODO: colocar spreadsheet number no modifier.

};

}

#endif /* HORUSEYE_FRAMEWORK_MODIFIER_H_ */
