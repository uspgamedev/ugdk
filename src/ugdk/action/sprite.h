#ifndef HORUSEYE_FRAMEWORK_SPRITE_H_
#define HORUSEYE_FRAMEWORK_SPRITE_H_

#include <algorithm>
#include <ugdk/math/vector2D.h>
#include <ugdk/graphic/drawable.h>
#include <ugdk/graphic/image.h>
#include <ugdk/base/types.h>
#include <ugdk/action/animation.h>


namespace ugdk {

class Drawable;
class Modifier;
class Light;
class AnimationSet;
class Node;

class Sprite : public Drawable {
  public:
    Sprite();

    /// This modifier will be deleted together with the Sprite.
    Sprite(Modifier *modifier);

    virtual ~Sprite();

    void Draw();
    void set_node(Node* node) { node_ = node; }

    /// Initializes the Sprite with a drawable to render and an AnimationSet.
    /** If no AnimationSet is defined, the first frame will be used.*/
    virtual void Initialize(Drawable *image, AnimationSet *set = NULL,
                    bool delete_image = false);

    /// Acessors e mutators
    Vector2D position() const { return position_; }
    void set_position(const Vector2D& position) { position_ = position; }
    void set_position(float x, float y) { position_ = Vector2D(x, y); }

    /// Returns if the sprite is visible or not.
    /**Visibility controls wether the Sprite is rendered or not. This function returns
       a boolean value if the sprite is visible or not.
    *  @return boolean value visible
    */
    bool visible() const { return visible_; }

    /// Set if the sprite is visible or not.
    /** This function recive a boolean value and set if the sprite is visible or not.
    *  @param visible is a boolean value
    *  @see visible()
    */
    void set_visible(bool visible) { visible_ = visible; }
    
    /// Return the hotspot of the sprite
    /** The hotspot controls the offset from the image origin to the sprite position.
    *  @return Vector2D of the hotspot
    */
    Vector2D hotspot() const { return hotspot_; }
    /// Set the hotspot of the sprite
    /** Given a hotspot Vector2D, this function sets the hotspot of the sprite
    *  @param hotspost is a Vector2D 
    *  @see hotspot()
    */
    void set_hotspot(const Vector2D& hotspot) { hotspot_ = hotspot; }
    /// Set the hotspot of the sprite
    /** Given two float (x,y), this function sets the hotspot of the sprite
    *  @param x is the x coordinate of the hotspot
    *  @param y is the y coordinate of the hotspot
    *  @see hotspot()
    */
    void set_hotspot(float x, float y) { hotspot_ = Vector2D(x, y); }

    ///  Return the zindex of the sprite
    /** The Sprites are rendered in order, with non-decreasing Z-Index.
    *  @return zindex
    */
    float zindex() const { return zindex_; }
    /// Set the zindex of the sprite
    /** @param z is a float
    *  @see zindex()
    */
    void set_zindex(float z) { zindex_= z; }
    static bool CompareByZIndex(Sprite *a, Sprite *b);

        /// Return the modifier color
        /** @return Color object
        */ 
	Color color() { return modifier_->color(); }
        /// Set the modifier color
        /** @param color is a Color object
        */ 
	void set_color(Color color) { modifier_->set_color(color); }
	
        /// Return alpha
        /** @return alpha is a float*/
	float alpha() { return modifier_->alpha(); }
        /// Set alpha
        /** @param alpha is a float*/
	void set_alpha(float alpha) { modifier_->set_alpha(alpha); }

        /// Return the Vector2D size of the sprite
        /** @return size_ is a const*/
	const Vector2D size() { return size_; }
        /// Set the size of the Vector2D
        /** @param size is a Vector2D size*/
	void set_size(const Vector2D& size) { size_ = size; }
	
    /// Return a pointer to the light object an animation
    /** The light determines how much this Sprite illuminates the ambient. 
        By default, a Sprite has no light.
    *  @return light pointer
    */
	Light* light() { return light_; }
    
    /// Set a new light to the animation
    /** Given a light pointer, this function switches the current light of the animation 
        to the new one given.
    *  @param pointer to a light object
    */
	void set_light(Light* light) { light_ = light; }

    /// Return the mirror of the sprite
    /** The mirror ammount is combined with whatever mirror the image or the animation has. */
    Mirror mirror() const { return modifier_->mirror(); }
    void set_mirror(Mirror mirror) { modifier_->set_mirror(mirror); }

    /// Change the current animation to a new animation from the previously selected AnimationSet.
    /**Given a animation name (a string), the function changes the current animation to a new animation of AnimationSet*/
    void SelectAnimation(std::string animation_name) {
        animation_manager_->Select(animation_name);
    }
    /// Change the current animation to a new animation from the previously selected AnimationSet.
    /**Given a animation index (a integer), the function changes the current animation to a new animation of AnimationSet*/
    void SelectAnimation(int animation_index) {
        animation_manager_->Select(animation_index);
    }

    /// Change the SpeedupFactor of the animation
    /** Given a a factor, this function set the animation SpeedupFactor to this value
    *  @param factor is a float 
    */
    void SetAnimationSpeedupFactor(float factor)  { animation_manager_->set_speedup_factor(factor);  }

    /// Change the SlowdownFactor of the animation
    /** Given a a factor, this function set the animation SlowdownFactor to this value
    *  @param factor is a float 
    */
    void SetAnimationSlowdownFactor(float factor) { animation_manager_->set_slowdown_factor(factor); }

    /// Return the animation FPS
    /** Return the animation FPS
    *  @return	a float equivalente to the animation FPS
    */
    float GetAnimationFPS() const { return animation_manager_->fps(); }

    /// Add a observer object to the animation
    /** Given an observer object, the function include this in the animation manager
    *  @param *observer is a pointer to the observer object
    */
    void AddObserverToAnimation(Observer *observer) {
        animation_manager_->AddObserver(observer);
    }

    /// Return the animation frame number
    /** @return an integer that is the animation frame number
    */
    int GetAnimationFrameNumber() const { return animation_manager_->n_frames(); }
    /// Set the default frame
    /** @param frama is a integer
    */
    void SetDefaultFrame(int frame) {
        animation_manager_->set_default_frame(frame);
    }

    virtual void Render();

    /// Illuminate the ambient.
    /**All lights are rendered before any actual image.*/
	void RenderLight(Vector2D &offset);

    /// Update the Sprite based on the time variation.
    /** One of the two main functions of the UGDK Engine. Most of the game logic 
        resides within the Update of child classes.*/
    virtual void Update(float delta_t);

  protected:
    Light *light_;
    float zindex_;

  private:
    Vector2D position_, hotspot_, size_;
    Drawable *image_;
    AnimationManager *animation_manager_;
    Modifier *modifier_;
    bool visible_, delete_image_;
    Node* node_;

};

}

#endif //HORUSEYE_FRAMEWORK_SPRITE_H_
