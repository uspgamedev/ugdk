#ifndef HORUSEYE_FRAMEWORK_LAYER_H_
#define HORUSEYE_FRAMEWORK_LAYER_H_

#include <vector>
#include <map>
#include <ugdk/base/types.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/action/sprite.h>
#include <ugdk/graphic/node.h>

namespace ugdk {

/// Represents a graphic layer where the sprites will be drawn
/** A Layer object can have an offset, that is the shift of the hole layer in relation to the origin of the screen (fullscreen) or the window
*/
class Layer {
  public:
    /// Consructor of the layer class.
    Layer(Vector2D offset = Vector2D()) : node_(new Node) { node_->modifier()->set_offset(offset); }
    virtual ~Layer();

    /// Manage the atributes.
    /// Return if the layer is visible
    /**  @return visible is a bool variable
    */  
    bool IsVisible() { return node_->visible(); }
    /// Set if the layer is visible 
    /** @param visible is a bool variable
    */
    void set_visible(bool visible) { node_->set_visible(visible); }
    /// Set the offset
    /** @param  offset is a Vector2D
    */ 
    void set_offset(Vector2D offset) { node_->modifier()->set_offset(offset); }
    /// Return the offset
    /** @return  offset is a Vector2D object
    */ 
    Vector2D offset() { return node_->modifier()->offset(); }

    /// Add a sprite to the Layer
    /** @param sprite is a pointer to a sprite*/
    void AddSprite(Sprite *sprite);

    /// Remove a sprite from the Layer
    /** @param sprite is a pointer to a sprite*/
    void RemoveSprite(Sprite *sprite);

    /// Update the layer and it sprites 
    virtual void Update(float delta_t);

    /// Draw the layer
    virtual void Render() { node_->Render(); }

    /// Render the light
    virtual void RenderLight() { node_->RenderLight(); }

  protected:
    /// Basic structure to store the Sprites. Not definitive.
    std::map<Sprite*, Node*> sprite_list_;

  private:
    Node* node_;

    /// Sort sprites using zindex
    virtual void SortSprites();

}; // class Layer.

}

#endif // HORUSEYE_FRAMEWORK_LAYER_H_
