
#include <ugdk/graphic/modifier.h>
#include <ugdk/graphic/image.h>
#include <algorithm>

#define TWO_PI 6.28318530f
#define TO_UNIT_INTERVAL(FLOAT) ( std::min(std::max(FLOAT, 0.0f), 1.0f) )

namespace ugdk {

Modifier::Modifier(Modifier& mod) :
        offset_(mod.offset_),
        scale_(mod.scale_),
        rotation_(mod.rotation_),
        mirror_(mod.mirror_),
        color_(mod.color_),
        alpha_(mod.alpha_) {}

void Modifier::Compose(const Modifier* mod2) {
    this->ComposeOffset(mod2);
    this->ComposeScale(mod2);
    this->ComposeRotation(mod2);
    this->ComposeMirror(mod2);
    this->ComposeColor(mod2);
    this->ComposeAlpha(mod2);
}




void Modifier::set_mirror(const Mirror mirror) {
    if( mirror == MIRROR_HFLIP || mirror == MIRROR_VFLIP )
        mirror_  = mirror;
    else mirror_ = MIRROR_NONE;
}

void Modifier::set_color(const Color color) {
    color_.r = TO_UNIT_INTERVAL(color.r);
    color_.g = TO_UNIT_INTERVAL(color.g);
    color_.b = TO_UNIT_INTERVAL(color.b);
}

void Modifier::set_rotation(const float rotation) {
    rotation_ = std::fmod(rotation,TWO_PI);
}

void Modifier::set_alpha(const float alpha) {
    alpha_ = TO_UNIT_INTERVAL(alpha);
}


void Modifier::ComposeMirror(const Mirror mirror) {
    if( mirror == MIRROR_HFLIP || mirror == MIRROR_VFLIP )
        mirror_ ^= mirror;
}

void Modifier::ComposeColor(const Color& color) {
    color_.r *= TO_UNIT_INTERVAL(color.r);
    color_.g *= TO_UNIT_INTERVAL(color.g);
    color_.b *= TO_UNIT_INTERVAL(color.b);
}

void Modifier::ComposeRotation(const float rotation) {
    rotation_ *= std::fmod(rotation,TWO_PI);
}

void Modifier::ComposeAlpha(const float alpha) {
    alpha_ *= TO_UNIT_INTERVAL(alpha);
}

} // namespace ugdk
