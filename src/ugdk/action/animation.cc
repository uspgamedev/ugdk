#include <ugdk/action/animation.h>
#include <ugdk/action/observer.h>
#include <ugdk/action/animationset.h>

namespace ugdk {

namespace action{

#define ANIMATION_BUFFER_SIZE 256

AnimationManager::AnimationManager(double fps, AnimationSet *set)
    : period_scaling_factor_(1.0), current_animation_(NULL), animation_set_(set),
      current_frame_(0), default_frame_(0) {}


AnimationManager::~AnimationManager() {}

int AnimationManager::GetFrame() {
    if (current_animation_) {
        AnimationFrame *frame = current_animation_->at(current_frame_);
        if (frame)
            return frame->frame();
        else return 0;
    }
    else return default_frame_;
}

void AnimationManager::Select(std::string name) {
    if (animation_set_) {
        Animation *new_frames = animation_set_->Search(name);
        if (current_animation_ != new_frames) {
            current_animation_ = new_frames;
            current_frame_ = 0;
            elapsed_time_ = 0;
        }
    }
}

void AnimationManager::Select(int index) {
    if (animation_set_) {
        Animation *new_frames = animation_set_->Get(index);
        if (current_animation_ != new_frames) {
            current_animation_ = new_frames;
            current_frame_ = 0;
            elapsed_time_ = 0;
        }
    }
}

void AnimationManager::Update(double delta_t) {
    if (!current_animation_) return;
    elapsed_time_ += delta_t;
    if (elapsed_time_ >= current_animation_->period()) {
        current_frame_ = (current_frame_ + 1) % current_animation_->size();
        if (current_frame_ == 0) NotifyAllObservers();
        elapsed_time_ -= current_animation_->period();
    }
}

void AnimationManager::AddObserver(Observer* observer) {
    observers.push_back(observer);
}

void AnimationManager::NotifyAllObservers() {
    for (int i = 0; i < (int)observers.size(); ++i) {
        observers[i]->Tick();
    }
}

} /* namespace action*/

} /* namespace ugdk */




