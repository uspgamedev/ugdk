#include <algorithm>
#include <ugdk/action/layer.h>
#include <ugdk/base/engine.h>
#include <ugdk/graphic/videomanager.h>

namespace ugdk {

Layer::~Layer() {
    delete node_; // This also clears all our nodes, for free!
}

void Layer::Update(float delta_t) {
    //SortSprites();

    std::map<Sprite*,Node*>::iterator it = sprite_list_.begin();
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
        Node* sprite_node = sprite_list_[sprite] = new Node(sprite);
        node_->AddChild(sprite_node);
    }
}

void Layer::RemoveSprite(Sprite* sprite) {

    Node* sprite_node = sprite_list_[sprite];
    if(sprite_node != NULL) {
        sprite_list_[sprite] = NULL;
        node_->RemoveChild(sprite_node);
        delete sprite_node;
    }
}

void Layer::SortSprites() {
    // ordena sprites pelo valor de zindex
    //std::sort(sprite_list_.begin(), sprite_list_.end(), Sprite::CompareByZIndex);

    // TODO: SCREW THIS SHIT, NO ORDERING FOR U
}

}
