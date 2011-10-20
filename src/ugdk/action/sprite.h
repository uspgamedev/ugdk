#ifndef HORUSEYE_FRAMEWORK_SPRITE_H_
#define HORUSEYE_FRAMEWORK_SPRITE_H_

#include <algorithm>
#include <ugdk/math/vector2D.h>
#include <ugdk/graphic/image.h>
#include <ugdk/base/types.h>
#include <ugdk/action/animation.h>


namespace ugdk {

class Drawable;
class Modifier;
class Light;
class AnimationSet;

class Sprite {
  public:
    Sprite();

    // This modifier will be deleted together with the Sprite.
    Sprite(Modifier *modifier);

    virtual ~Sprite();

    // Initializes the Sprite with a drawable to render and an AnimationSet.
    // If no AnimationSet is defined, the first frame will be used.
    virtual void Initialize(Drawable *image, AnimationSet *set = NULL,
                    bool delete_image = false);

    // Acessors e mutators
    Vector2D position() const { return position_; }
    void set_position(const Vector2D& position) { position_ = position; }
    void set_position(float x, float y) { position_ = Vector2D(x, y); }

    // Visibility controls wether the Sprite is rendered or not.
    bool visible() const { return visible_; }
    void set_visible(bool visible) { visible_ = visible; }

    // The hotspot controls the offset from the image origin to the sprite position.
    Vector2D hotspot() const { return hotspot_; }
    void set_hotspot(const Vector2D& hotspot) { hotspot_ = hotspot; }
    void set_hotspot(float x, float y) { hotspot_ = Vector2D(x, y); }

    // The Sprites are rendered in order, with non-decreasing Z-Index. 
    float zindex() const { return zindex_; }
    void set_zindex(float z) { zindex_= z; }
    static bool CompareByZIndex(Sprite *a, Sprite *b);


	Color color() { return modifier_->color(); }
	void set_color(Color color) { modifier_->set_color(color); }
	
	float alpha() { return modifier_->alpha(); }
	void set_alpha(float alpha) { modifier_->set_alpha(alpha); }

	const Vector2D size() { return size_; }
	void set_size(const Vector2D& size) { size_ = size; }
	
    // The light determines how much this Sprite illuminates the ambient. 
    // By default, a Sprite has no light.
	Light* light() { return light_; }
	void set_light(Light* light) { light_ = light; }

    // The mirror ammount is combined with whatever mirror the image or the animation has.
    Mirror mirror() const { return modifier_->mirror(); }
    void set_mirror(Mirror mirror) { modifier_->set_mirror(mirror); }

    // Changes the current animation to a new animation from the previously selected AnimationSet.
    void SelectAnimation(std::string animation_name) {
        animation_manager_->Select(animation_name);
    }
    void SelectAnimation(int animation_index) {
        animation_manager_->Select(animation_index);
    }


    void SetAnimationSpeedupFactor(float factor)  { animation_manager_->set_speedup_factor(factor);  }
    void SetAnimationSlowdownFactor(float factor) { animation_manager_->set_slowdown_factor(factor); }
    float GetAnimationFPS() const { return animation_manager_->fps(); }


    void AddObserverToAnimation(Observer *observer) {
        animation_manager_->AddObserver(observer);
    }
    int GetAnimationFrameNumber() const { return animation_manager_->n_frames(); }
    void SetDefaultFrame(int frame) {
        animation_manager_->set_default_frame(frame);
    }

    virtual void Render();

    // Illuminates the ambient. All lights are rendered before any actual image.
	void RenderLight(Vector2D &offset);

    // Updates the Sprite based on the time variation.
    // One of the two main functions of the UGDK Engine. Most of the game logic 
    // resides within the Update of child classes.
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

};
}

#endif //HORUSEYE_FRAMEWORK_SPRITE_H_
