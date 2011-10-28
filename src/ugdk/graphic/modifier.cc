
#include <ugdk/graphic/modifier.h>
#include <ugdk/graphic/image.h>
#include <algorithm>

#define TWO_PI 6.28318530f

namespace ugdk {

Modifier::Modifier (Modifier &mod) :
        offset_(mod.offset_),
        scale_(mod.scale_),
        rotation_(mod.rotation_),
        mirror_(mod.mirror_),
        color_(mod.color_),
        alpha_(mod.alpha_) {}

void Modifier::Compose(const Modifier *mod) {
    offset_ = offset_ + mod->offset_;
    scale_ = Vector2D(scale_.x*mod->scale_.x,
                     scale_.y*mod->scale_.y);
    rotation_ *= mod->rotation_;
    mirror_ ^= mod->mirror_;
    color_.r *= mod->color_.r;
    color_.g *= mod->color_.g;
    color_.b *= mod->color_.b;
    alpha_ *= mod->alpha_;
}




void Modifier::set_mirror(const Mirror mirror) {
    if( mirror == MIRROR_HFLIP || mirror == MIRROR_VFLIP )
        mirror_  = mirror;
    else mirror_ = MIRROR_NONE;
}

void Modifier::set_rotation(const float rotation) {
    rotation_ = std::fmod(rotation,TWO_PI);
}

void Modifier::set_alpha(const float alpha) {
    alpha_ = std::min( std::max(alpha, 0.0f), 1.0f );
}


void Modifier::ComposeMirror(const Mirror mirror) {
    if( mirror == MIRROR_HFLIP || mirror == MIRROR_VFLIP )
        mirror_ ^= mirror;
}

void Modifier::ComposeRotation(const float rotation) {
    rotation_ *= std::fmod(rotation,TWO_PI);
}

void Modifier::ComposeAlpha(const float alpha) {
    alpha_ *= std::min( std::max(alpha, 0.0f), 1.0f );
}

} // namespace ugdk
