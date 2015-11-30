#ifndef UGDK_ACTION_ANIMATION_H_
#define UGDK_ACTION_ANIMATION_H_

#include <ugdk/action/mediaplayer.h>
#include <ugdk/structure/indexabletable.h>

#include <string>
#include <vector>
#include <functional>

namespace ugdk {
namespace action {

template<class Frame>
class Animation {
  public:
    using value_type = Frame;
    using Vector = std::vector<std::unique_ptr<Frame>>;

    enum class AnimationRepeat {
        LOOP, NONE
    };

    template <class ...Args>
    Animation(Args... args)
        : frames_(std::forward(args)...)
        , repeat_(AnimationRepeat::LOOP)
    {}
    ~Animation() {}

    Vector& frames() { return frames_; }
    const Vector& frames() const { return frames_; }

    AnimationRepeat repeat() const { return repeat_; }
    void set_repeat(AnimationRepeat repeat) { repeat_ = repeat; }

  private:
    Vector frames_;
    AnimationRepeat repeat_;
};

} /* namespace action */
} /* namespace ugdk */

#endif /* UGDK_ACTION_ANIMATION_H_ */


