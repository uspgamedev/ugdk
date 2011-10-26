#include <ugdk/action/sprite.h>
#include <ugdk/graphic/drawable.h>
#include <ugdk/graphic/light.h>
#include <ugdk/action/animation.h>

namespace ugdk {

Sprite::Sprite() : light_(NULL), image_(NULL), animation_manager_(NULL),
    modifier_(new Modifier), delete_image_(false) {}

Sprite::Sprite(Modifier *mod) : light_(NULL), image_(NULL), animation_manager_(NULL),
    modifier_(mod), delete_image_(false) {}

Sprite::~Sprite() {
    if (animation_manager_) delete animation_manager_;
    if (delete_image_ && image_) delete image_;
    if (modifier_) delete modifier_;
}

void Sprite::Initialize(Drawable *image, AnimationSet *set, bool delete_image)
{
    image_ = image;
  	set_zindex(0.0f);
    visible_ = true;
    animation_manager_ = new AnimationManager(10, set);/*TODO: MANO TEM UM 10 NO MEU C�DIGO */
    hotspot_ = position_ = Vector2D(0,0);
    delete_image_ = delete_image;
	size_ = image->render_size();
}

void Sprite::Render() {
    if (visible_) {
        int frame_number = animation_manager_->GetFrame();
        Modifier render_mod(*modifier_);
        const Modifier *animation_mod = animation_manager_->get_current_modifier();
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
    animation_manager_->Update(delta_t);
}

bool Sprite::CompareByZIndex(Sprite *a, Sprite *b) {
    return a->zindex() < b->zindex();
}

}
