#include <ugdk/action/sprite.h>
#include <ugdk/graphic/drawable.h>
#include <ugdk/graphic/light.h>
#include <ugdk/graphic/node.h>
#include <ugdk/action/animation.h>

namespace ugdk {

Sprite::Sprite() : light_(NULL), image_(NULL), spritesheet_(NULL), animation_manager_(NULL),
    modifier_(new Modifier), delete_image_(false), node_(NULL) {}

Sprite::Sprite(Modifier *mod) : light_(NULL), image_(NULL), spritesheet_(NULL), animation_manager_(NULL),
    modifier_(mod), delete_image_(false), node_(NULL) {}

Sprite::~Sprite() {
    if (animation_manager_) delete animation_manager_;
    if (delete_image_ && image_) delete image_;
    if (modifier_) delete modifier_;
}

void Sprite::Draw() {
    Render();
}

void Sprite::Initialize(Drawable *image, AnimationSet *set, bool delete_image)
{
    image_ = image;
  	set_zindex(0.0f);
    visible_ = true;
    animation_manager_ = new AnimationManager(10, set);/*TODO: MANO TEM UM 10 NO MEU C�DIGO */
    hotspot_ = Vector2D(0,0);
    delete_image_ = delete_image;
	size_ = image->render_size();
}

void Sprite::Initialize(Spritesheet *spritesheet, AnimationSet *set)
{
    spritesheet_ = spritesheet;
  	set_zindex(0.0f);
    visible_ = true;
    animation_manager_ = new AnimationManager(10, set);/*TODO: MANO TEM UM 10 NO MEU C�DIGO */
    hotspot_ = Vector2D(0,0);
}

void Sprite::Render() {
    if (visible_) {
        int frame_number = animation_manager_->GetFrame();

        Modifier render_mod(*modifier_);
        const Modifier *animation_mod = animation_manager_->get_current_modifier();
        if (animation_mod) render_mod.Compose(animation_mod);
        if(image_) {
            Vector2D true_hotspot = hotspot_.Scale(render_mod.scale());
            image_->DrawTo(Vector2D() - true_hotspot, frame_number, &render_mod, size_);
        } else {
            spritesheet_->Draw(frame_number);
        }
    }
}

void Sprite::RenderLight(Vector2D &offset) {
    if (visible_ && light_) {
		Vector2D pos = - offset;
        light_->Render(pos);
    }
}

void Sprite::Update(float delta_t) {
    animation_manager_->Update(delta_t);
    if(node_) node_->set_zindex(this->zindex());
}

bool Sprite::CompareByZIndex(Sprite *a, Sprite *b) {
    return a->zindex() < b->zindex();
}

}
