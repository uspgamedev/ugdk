#include <algorithm>
#include <ugdk/action/layer.h>
#include <ugdk/action/sprite.h>
#include <ugdk/base/engine.h>
#include <ugdk/graphic/videomanager.h>

namespace ugdk {

Layer::~Layer() {
    delete node_; // This also clears all our nodes, for free!
}

void Layer::Update(float delta_t) {
    SortSprites();

    std::list< std::pair<Sprite*, Node*> >::iterator it = sprite_list_.begin();
    while (it != sprite_list_.end()) {
        it->first->Update(delta_t);
        ++it;
    }
}

/*
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
*/

void Layer::AddSprite(Sprite* sprite) {
    if(sprite != NULL) {
        Node* sprite_node = new Node(sprite);
        sprite_list_.push_front(std::pair<Sprite*, Node*>(sprite, sprite_node));
        node_->AddChild(sprite_node);
        sprite->set_node(sprite_node);
    }
}

void Layer::RemoveSprite(Sprite* sprite) {
    std::list< std::pair<Sprite*, Node*> >::iterator it;
    for(it = sprite_list_.begin(); it != sprite_list_.end(); ++it) {
        if(it->first != sprite) continue;
        node_->RemoveChild(it->second);
        delete it->second;
        sprite_list_.erase(it);
        return;
    }
}

/*bool ListCompareFunc(std::pair<Sprite*, Node*> a, std::pair<Sprite*, Node*> b) {
    return Sprite::CompareByZIndex(a.first, b.first);
}*/

void Layer::SortSprites() {
    node_->SortChildren();
}

}
