#ifndef HORUSEYE_FRAMEWORK_LAYER_H_
#define HORUSEYE_FRAMEWORK_LAYER_H_

#include <vector>
#include <ugdk/base/types.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/action/sprite.h>

namespace ugdk {

/// Represents a graphic layer where the sprites will be drawn
/** A Layer object can have an offset, that is the shift of the hole layer in relation to the origin of the screen (fullscreen) or the window
*/
class Layer {
  public:
    /// Consructor of the layer class.
    Layer(Vector2D offset = Vector2D()) : visible_(true), light_type_(LIGHT_IGNORE) {}
    virtual ~Layer();

    /// Manage the atributes.
    /// Return if the layer is visible
    /**  @return visible is a bool variable
    */  
    bool IsVisible() { return visible_; }
    /// Set if the layer is visible 
    /** @param visible is a bool variable
    */
    void set_visible(bool visible) { visible_ = visible; }
    /// Set the offset
    /** @param  offset is a Vector2D
    */ 
    void set_offset(Vector2D offset) { offset_ = offset; }
    /// Return the offset
    /** @return  offset is a Vector2D object
    */ 
    Vector2D offset() { return offset_; }
    /// Set the light type
    /** @param type is a LightType object*/
    void set_light_type(LightType type) { light_type_ = type; }
    /// Return the light type
    /** @return light_type_ is a LightType object*/
    LightType light_type() { return light_type_; }

    /// Add a sprite to the Layer
    /** @param sprite is a pointer to a sprite*/
    void AddSprite(Sprite *sprite);
    /// Remove a sprite from the Layer
    /** @param sprite is a pointer to a sprite*/
    void RemoveSprite(Sprite *sprite);

    /// Update the layer and it sprites 
    virtual void Update(float delta_t);

    /// Draw the layer
    virtual void Render();
        /// Render the light
	virtual void RenderLight();

  protected:
    /// Basic structure to store the Sprites. Not definitive.
    std::vector<Sprite*> sprite_list_;
    bool visible_;

  private:
    /// Atributes.
    Vector2D offset_;

    LightType light_type_;

    /// Sort sprites using zindex
    virtual void SortSprites();

}; // class Layer.

}

#endif // HORUSEYE_FRAMEWORK_LAYER_H_
