#ifndef UGDK_ACTION_ANIMATIONPLAYER_H_
#define UGDK_ACTION_ANIMATIONPLAYER_H_

#include <ugdk/action/mediaplayer.h>
#include <ugdk/structure/indexabletable.h>
#include <ugdk/action/animation.h>

#include <string>
#include <vector>
#include <functional>

namespace ugdk {
namespace action {

template<class Frame>
class AnimationPlayer : public MediaPlayer {
  public:
    using Animation = ::ugdk::action::Animation<Frame>;
    using AnimationTable = structure::IndexableTable<Animation>;
    using FrameChangedCallback = std::function<void(const Frame&)>;

    explicit AnimationPlayer(const std::shared_ptr<AnimationTable> table)
        : current_animation_(nullptr)
        , current_frame_(0)
        , elapsed_time_(0.0)
        , table_(table)
        , notified_(false)
    {}

    void set_frame_change_callback(const FrameChangedCallback& callback) {
        frame_change_callback_ = callback;
    }

    const shared_ptr<Animation> current_animation() const {
        return current_animation_;
    }

    const Frame& current_animation_frame() const {
        return *current_animation_->frames()[current_frame_];
    }

    void Update(double dt) override {
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
        notified_ = false;
        elapsed_time_  = 0.0;
        current_frame_ = 0;
        ExecuteFrameChangeCallback();
    }

    /// Change the current animation to a new animation from the previously selected AnimationSet.
    /**Given a animation name (a string), the function changes the current animation to a new animation of AnimationSet*/
    void Select(const std::string& name, bool reset_animation_if_different = true) {
        if(table_)
            this->set_current_animation(table_->Search(name), reset_animation_if_different);
    }

    /// Change the current animation to a new animation from the previo2usly selected AnimationSet.
    /**Given a animation index (a integer), the function changes the current animation to a new animation of AnimationSet*/
    void Select(int index, bool reset_animation_if_different = true) {
        if(table_)
            this->set_current_animation(table_->Get(index), reset_animation_if_different);
    }

    /// Calls the frame changed callback with the current frame.
    void Refresh() {
        if (current_animation_)
            ExecuteFrameChangeCallback();
    }

  private:
    const shared_ptr<Animation> current_animation_;
    int current_frame_;
    double elapsed_time_;
    const shared_ptr<AnimationTable> table_;
    FrameChangedCallback frame_change_callback_;
    bool notified_;

    void set_current_animation(const shared_ptr<Animation> anim, bool reset_animation_if_different) {
        auto previous_anim = current_animation_;
        current_animation_ = anim;

        if (anim != previous_anim) {
            if (reset_animation_if_different || current_frame_ >= current_animation_->frames().size()) {
                RestartAnimation();
            } else {
                ExecuteFrameChangeCallback();
            }
        }
    }

    void ExecuteFrameChangeCallback() {
        if (frame_change_callback_)
            frame_change_callback_(current_animation_frame());
    }

    void ChangeToNextFrame() {
        bool notify = false;

        ++current_frame_;
        if(current_frame_ >= current_animation_->frames().size()) {
            if (current_animation_->repeat() == Animation::AnimationRepeat::LOOP) {
                current_frame_ = 0;
                notified_ = false;
            }
            else {
                --current_frame_;
            }
            notify = true;
        }
        ExecuteFrameChangeCallback();
            
        if (notify && !notified_) {
            notifyAllObservers();
            notified_ = true;
        }
    }
};

} /* namespace action */
} /* namespace ugdk */

#endif /* UGDK_ACTION_SPECIALIZEDANIMATIONMANAGER_H_ */


