#include <sstream>
#include <algorithm>
#include <cmath>

#include <ugdk/graphic/modifier.h>

#define TWO_PI 6.283185307179586476925286766559
#define TO_UNIT_INTERVAL(double) ( std::min(std::max((double), 0.0), 1.0) )

namespace ugdk {

namespace graphic {

Modifier::Modifier() : offset_(), scale_(1.0, 1.0), rotation_(0.0), 
    mirror_(MIRROR_NONE), color_(WHITE), visible_(true), flags_(0) {}

void Modifier::Compose(const Modifier* mod2) {

    if(mod2 == nullptr) return;

    this->ComposeOffset(   mod2->offset_   );
    this->ComposeScale(    mod2->scale_    );
    this->ComposeRotation( mod2->rotation_ );
    this->ComposeMirror(   mod2->mirror_   );
    this->ComposeColor(    mod2->color_    );
    this->ComposeVisible(  mod2->visible_  );
    flags_ |= mod2->flags_;
}

Modifier* Modifier::Compose(const Modifier* mod1, const Modifier* mod2) {
    if(mod1 == nullptr) return Copy(mod2);

    Modifier* mod = Copy(mod1);
    mod->Compose(mod2);
    return mod;
}



void Modifier::set_mirror(const Mirror mirror) {
    if( (mirror & MIRROR_HFLIP) || (mirror & MIRROR_VFLIP) )
        mirror_  = mirror;
    else
        mirror_ = MIRROR_NONE;
}

void Modifier::set_color(const Color& color) {
    color_ = color;
    flags_ |= HAS_COLOR;
}

void Modifier::set_rotation(const double rotation) {
    rotation_ = fmod(rotation,TWO_PI);
    flags_ |= HAS_TRANSFORMATION;
}

void Modifier::ComposeMirror(const Mirror& mirror) {
    if( (mirror & MIRROR_HFLIP) || (mirror & MIRROR_VFLIP) )
        mirror_ ^= mirror;
}

void Modifier::ComposeColor(const Color& color) {
    color_.Compose(color);
    flags_ |= HAS_COLOR;
}

void Modifier::ComposeRotation(const double rotation) {
    rotation_ += fmod(rotation,TWO_PI);
    flags_ |= HAS_TRANSFORMATION;
}

Modifier* Modifier::Copy(const Modifier* mod2) { 
    if(mod2 == nullptr) return nullptr;
    return new Modifier(*mod2);
}

}  // namespace graphic
}  // namespace ugdk
