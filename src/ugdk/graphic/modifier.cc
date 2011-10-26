
#include <ugdk/graphic/modifier.h>
#include <ugdk/graphic/image.h>

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

}
