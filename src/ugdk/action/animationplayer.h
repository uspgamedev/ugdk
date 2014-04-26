#ifndef UGDK_ACTION_ANIMATIONPLAYER_H_
#define UGDK_ACTION_ANIMATIONPLAYER_H_

#include <ugdk/action.h>
#include <ugdk/graphic.h>
#include <ugdk/action/mediaplayer.h>
#include <ugdk/structure/indexabletable.h>

#include <string>
#include <functional>
#include <cassert>

namespace ugdk {
namespace action {

template<class T>
class AnimationPlayer : public MediaPlayer {
  public:
    typedef std::function<void(const typename T::Frame&)> FrameChangedCallback;

    AnimationPlayer(const structure::IndexableTable<T*> *table) 
        : current_animation_(nullptr), current_frame_(0), elapsed_time_(0.0), table_(table) {
        // TODO: a user defined current_animation_ may leak
    }

    void set_frame_change_callback(const FrameChangedCallback& callback) {
        frame_change_callback_ = callback;
    }

    void set_current_animation(T* anim) {
        const T* previous_anim = current_animation_;
        current_animation_ = anim;
        if (anim != previous_anim)
            RestartAnimation();
    }

    const T* current_animation() const { return current_animation_; }

    const typename T::Frame& current_animation_frame() const {
        if(!current_animation() || current_frame_ < 0 || 
           current_frame_ >= static_cast<int>(current_animation()->size()))
            return T::Frame::DEFAULT();
        return current_animation()->At(current_frame_);
    }

    void Update(double dt) {
        if (!current_animation_) return;
        elapsed_time_ += dt;
        if (elapsed_time_ >= current_animation_->period()) {
            elapsed_time_ -= current_animation_->period();

            ChangeToNextFrame();
        }
    }

    /// Restarts the current animation from the first frame.
    void RestartAnimation() {
        elapsed_time_ = 0.0;
        ChangeCurrentFrame(0);
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

  private:
    const T* current_animation_;
    int current_frame_;
    double elapsed_time_;
    const structure::IndexableTable<T*> *table_;
    FrameChangedCallback frame_change_callback_;

    void ChangeCurrentFrame(int new_frame) {
        current_frame_ = new_frame;
        if (frame_change_callback_)
            frame_change_callback_(current_animation_frame());
    }

    void ChangeToNextFrame() {
        ChangeCurrentFrame((current_frame_ + 1) % current_animation_->size());
        if (current_frame_ == 0) notifyAllObservers();
    }
};

} /* namespace action */
} /* namespace ugdk */

#endif /* UGDK_ACTION_SPECIALIZEDANIMATIONMANAGER_H_ */


