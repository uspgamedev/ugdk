#include <sstream>
#include <algorithm>
#include <cmath>

#include <ugdk/graphic/modifier.h>

#define TWO_PI 6.28318530
#define TO_UNIT_INTERVAL(double) ( std::min(std::max((double), 0.0), 1.0) )

namespace ugdk {

namespace graphic {

Modifier::Modifier() : offset_(), scale_(1.0, 1.0), rotation_(0.0), 
    mirror_(MIRROR_NONE), color_(WHITE), visible_(true), flags_(0) {}

void Modifier::Compose(const Modifier* mod2) {

    if(mod2 == NULL) return;

    this->ComposeOffset(   mod2->offset_   );
    this->ComposeScale(    mod2->scale_    );
    this->ComposeRotation( mod2->rotation_ );
    this->ComposeMirror(   mod2->mirror_   );
    this->ComposeColor(    mod2->color_    );
    this->ComposeVisible(  mod2->visible_  );
}

Modifier* Modifier::Compose(const Modifier* mod1, const Modifier* mod2) {

    if(mod1 == NULL) return Copy(mod2);

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
    color_.r = TO_UNIT_INTERVAL(color.r);
    color_.g = TO_UNIT_INTERVAL(color.g);
    color_.b = TO_UNIT_INTERVAL(color.b);
    color_.a = TO_UNIT_INTERVAL(color.a);
    flags_ |= HAS_COLOR;
}

void Modifier::set_rotation(const double rotation) {
    rotation_ = fmod(rotation,TWO_PI);
    flags_ |= HAS_TRANSFORMATION;
}

void Modifier::set_alpha(const double alpha) {
    color_.a = TO_UNIT_INTERVAL(alpha);
    flags_ |= HAS_COLOR;
}


void Modifier::ComposeMirror(const Mirror& mirror) {
    if( (mirror & MIRROR_HFLIP) || (mirror & MIRROR_VFLIP) )
        mirror_ ^= mirror;
}

void Modifier::ComposeColor(const Color& color) {
    color_.r *= TO_UNIT_INTERVAL(color.r);
    color_.g *= TO_UNIT_INTERVAL(color.g);
    color_.b *= TO_UNIT_INTERVAL(color.b);
    color_.a *= TO_UNIT_INTERVAL(color.a);
    flags_ |= HAS_COLOR;
}

void Modifier::ComposeRotation(const double rotation) {
    rotation_ += fmod(rotation,TWO_PI);
    flags_ |= HAS_TRANSFORMATION;
}

void Modifier::ComposeAlpha(const double alpha) {
    color_.a *= TO_UNIT_INTERVAL(alpha);
    flags_ |= HAS_COLOR;
}
    
Modifier* Modifier::Copy(const Modifier* mod2) { 
    if(mod2 == NULL) return NULL;
    return new Modifier(*mod2);
}

}  // namespace graphic
}  // namespace ugdk
