#include <algorithm>
#include <ugdk/action/layer.h>
#include <ugdk/base/engine.h>
#include <ugdk/graphic/videomanager.h>

namespace ugdk {

Layer::~Layer() {

    std::vector<Sprite*>::iterator it = sprite_list_.begin();
    while (it != sprite_list_.end()) {
        delete (*it);
        ++it;
    }

}

void Layer::Update(float delta_t) {

    std::vector<Sprite*>::iterator it = sprite_list_.begin();

    SortSprites();

    while (it != sprite_list_.end()) {
        (*it)->Update(delta_t);
        ++it;
    }
}

void Layer::Render() {
    if(visible_) {
        std::vector<Sprite*>::iterator it = sprite_list_.begin();
        Vector2D pos = Vector2D()-offset_;
        VIDEO_MANAGER()->TranslateTo(pos);
        while (it != sprite_list_.end()) {
            (*it)->Render();
            ++it;
        }
    }
}
void Layer::RenderLight() {
    if(visible_) {
        std::vector<Sprite*>::iterator it = sprite_list_.begin();
        while (it != sprite_list_.end()) {
			(*it)->RenderLight(offset_);
            ++it;
        }
    }
}

void Layer::AddSprite(Sprite* sprite)
{
    if(sprite != NULL)
        sprite_list_.push_back(sprite);
}

void Layer::RemoveSprite(Sprite* sprite)
{
    // nao use RemoveSprite() se estiver iterando pela sprite_list_!
    std::vector<Sprite*>::iterator it;

    it = std::find(sprite_list_.begin(), sprite_list_.end(), sprite);
    if(it != sprite_list_.end())
        sprite_list_.erase(it);
}

void Layer::SortSprites() {
    // ordena sprites pelo valor de zindex
    std::sort(sprite_list_.begin(), sprite_list_.end(), Sprite::CompareByZIndex);
}

}
