#include "sprite.h"
#include "drawable.h"
#include "light.h"
#include "animation.h"

namespace ugdk {

Sprite::Sprite() : light_(NULL), image_(NULL), animation_(NULL),
    modifier_(new Modifier), delete_image_(false) {}

Sprite::Sprite(Modifier *mod) : light_(NULL), image_(NULL), animation_(NULL),
    modifier_(mod), delete_image_(false) {}

Sprite::~Sprite() {
    if (animation_) delete animation_;
    if (delete_image_ && image_) delete image_;
    if (modifier_) delete modifier_;
}

void Sprite::Initialize(Drawable *image, AnimationSet *set, bool delete_image)
{
    image_ = image;
  	set_zindex(0.0f);
    visible_ = true;
    animation_ = new Animation(10, set);
    hotspot_ = position_ = Vector2D(0,0);
    delete_image_ = delete_image;
	size_ = image->render_size();
}

void Sprite::Render() {
    if (visible_) {
        int frame_number = animation_->GetFrame();
        Modifier render_mod(*modifier_);
        const Modifier *animation_mod = animation_->get_current_modifier();
        if (animation_mod) render_mod.Compose(animation_mod);
        image_->DrawTo(position_ - hotspot_, frame_number, &render_mod, size_);
    }
}

void Sprite::RenderLight(Vector2D &offset) {
    if (visible_ && light_) {
		Vector2D pos = position_ - offset;
        light_->Render(pos);
    }
}

void Sprite::Update(float delta_t) {
    animation_->Update(delta_t);
}

bool Sprite::CompareByZIndex(Sprite *a, Sprite *b) {
    return a->zindex() < b->zindex();
}

}
