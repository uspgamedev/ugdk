#ifndef UGDK_ACTION_ANIMATIONPLAYER_H_
#define UGDK_ACTION_ANIMATIONPLAYER_H_

#include <ugdk/action/mediaplayer.h>
#include <ugdk/structure/indexabletable.h>

#include <string>
#include <vector>
#include <functional>

namespace ugdk {
namespace action {

template<class Frame>
class AnimationPlayer : public MediaPlayer {
  public:
    typedef std::vector<Frame*> Vector;
    typedef std::function<void(const Frame&)> FrameChangedCallback;

    AnimationPlayer(const structure::IndexableTable<Vector*> *table)
        : current_animation_(nullptr)
        , elapsed_time_(0.0)
        , table_(table) 
    {}

    void set_frame_change_callback(const FrameChangedCallback& callback) {
        frame_change_callback_ = callback;
    }

    const Vector* current_animation() const {
        return current_animation_;
    }

    const Frame& current_animation_frame() const {
        return **current_frame_;
    }

    void Update(double dt) {
        if (!current_animation_) return;
        elapsed_time_ += dt;

        double frame_period = current_animation_frame().period();
        if (elapsed_time_ >= frame_period) {
            elapsed_time_ -= frame_period;
            ChangeToNextFrame();
        }
    }

    /// Restarts the current animation from the first frame.
    void RestartAnimation() {
        elapsed_time_  = 0.0;
        current_frame_ = current_animation_->begin();
        ExecuteFrameChangeCallback();
    }

    /// Change the current animation to a new animation from the previously selected AnimationSet.
    /**Given a animation name (a string), the function changes the current animation to a new animation of AnimationSet*/
    void Select(const std::string& name) {
        if(table_)
            this->set_current_animation(table_->Search(name));
    }

    /// Change the current animation to a new animation from the previo2usly selected AnimationSet.
    /**Given a animation index (a integer), the function changes the current animation to a new animation of AnimationSet*/
    void Select(int index) {
        if(table_)
            this->set_current_animation(table_->Get(index));
    }

    /// Calls the frame changed callback with the current frame.
    void Refresh() {
        if (current_animation_)
            ExecuteFrameChangeCallback();
    }

  private:
    const Vector* current_animation_;
    typename Vector::const_iterator current_frame_;
    double elapsed_time_;
    const structure::IndexableTable<Vector*> *table_;
    FrameChangedCallback frame_change_callback_;

    void set_current_animation(const Vector* anim) {
        auto previous_anim = current_animation_;
        current_animation_ = anim;
        if (anim != previous_anim)
            RestartAnimation();
    }

    void ExecuteFrameChangeCallback() {
        if (frame_change_callback_)
            frame_change_callback_(current_animation_frame());
    }

    void ChangeToNextFrame() {
        bool notify = false;

        ++current_frame_;
        if(current_frame_ == current_animation_->end()) {
            current_frame_ = current_animation_->begin();
            notify = true;
        }
        ExecuteFrameChangeCallback();
            
        if(notify)
            notifyAllObservers();
    }
};

} /* namespace action */
} /* namespace ugdk */

#endif /* UGDK_ACTION_SPECIALIZEDANIMATIONMANAGER_H_ */


