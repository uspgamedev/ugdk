#ifndef UGDK_ACTION_ANIMATIONPLAYER_H_
#define UGDK_ACTION_ANIMATIONPLAYER_H_

#include <string>

#include <ugdk/action.h>
#include <ugdk/graphic.h>
#include <ugdk/action/mediaplayer.h>
#include <ugdk/util/indexabletable.h>

namespace ugdk {
namespace action {

template<class T>
class AnimationPlayer : public MediaPlayer {
public:
    AnimationPlayer(const util::IndexableTable<T*> *table)
        : table_(table) {}

    ~AnimationPlayer() {}

    const T* current_animation() const { return current_animation_; }

    const typename T::Frame* current_animation_frame() const {
        if(current_frame_ < 0 || current_frame_ >= current_animation()->size()) return NULL;
        return current_animation()->At(current_frame_);
    }

    void Update(double dt) {
        if (!current_animation_) return;
        elapsed_time_ += dt;
        if (elapsed_time_ >= current_animation_->period()) {
            elapsed_time_ -= current_animation_->period();

            current_frame_ = (current_frame_ + 1) % current_animation_->size();
            if (current_frame_ == 0) notifyAllObservers();
        }
    }

    /// Change the current animation to a new animation from the previously selected AnimationSet.
    /**Given a animation name (a string), the function changes the current animation to a new animation of AnimationSet*/
    void Select(const std::string& name) {
        set_current_animation(table_->Search(name));
    }

    /// Change the current animation to a new animation from the previo2usly selected AnimationSet.
    /**Given a animation index (a integer), the function changes the current animation to a new animation of AnimationSet*/
    void Select(int index) {
        set_current_animation(table_->Get(index));
    }

private:
    void set_current_animation(T* anim) {
        if(anim != current_animation_) {
            current_animation_ = anim;
            current_frame_ = 0;
            elapsed_time_ = 0.0;
        }
    }

    const T* current_animation_;
    int current_frame_;
    double elapsed_time_;

    const util::IndexableTable<T*> *table_;
};

} /* namespace action */
} /* namespace ugdk */

#endif /* UGDK_ACTION_SPECIALIZEDANIMATIONMANAGER_H_ */


