#include <ugdk/action/animationmanager.h>

#include <ugdk/action/observer.h>

namespace ugdk {
namespace action {
    
AnimationManager::AnimationManager() : current_animation_(NULL), current_frame_(0) {}

AnimationManager::~AnimationManager() {}

void AnimationManager::set_current_animation(const Animation* animation) {
    current_animation_ = animation;
    current_frame_ = 0;
    elapsed_time_ = 0.0;
}

void AnimationManager::Update(double dt) {
    if (!current_animation_) return;
    elapsed_time_ += dt;
    if (elapsed_time_ >= current_animation_->period()) {
        elapsed_time_ -= current_animation_->period();

        current_frame_ = (current_frame_ + 1) % current_animation_->size();
        if (current_frame_ == 0) notifyAllObservers();
    }
}

void AnimationManager::AddObserver(Observer* observer) {
    observers_.push_back(observer);
}

void AnimationManager::AddTickFunction(std::tr1::function<void (void)> tick) {
    ticks_.push_back(tick);
}

void AnimationManager::notifyAllObservers() {
    for (int i = 0; i < (int)observers_.size(); ++i) {
        observers_[i]->Tick();
    }
    for(std::vector< std::tr1::function<void (void)> >::iterator it = ticks_.begin();
        it != ticks_.end(); ++it)
        (*it)();
}

} /* namespace action*/
} /* namespace ugdk */




