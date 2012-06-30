#include <ugdk/action/animation.h>
#include <ugdk/action/observer.h>
#include <ugdk/action/animationset.h>
#include <ugdk/action/animationframe.h>

namespace ugdk {

namespace action{

#define ANIMATION_BUFFER_SIZE 256

AnimationManager::AnimationManager(double fps, AnimationSet *set)
    : period_scaling_factor_(1.0), current_animation_(NULL), animation_set_(set),
      current_frame_(0), default_frame_(0) {}


AnimationManager::~AnimationManager() {}

double AnimationManager::fps() const { 
    return 1.0/(period_scaling_factor_*current_animation_->period());
}

double AnimationManager::period() const { 
    return period_scaling_factor_*current_animation_->period();
}

unsigned int AnimationManager::n_frames() const { 
    return static_cast<unsigned int>(current_animation_->size());
}

int AnimationManager::GetFrame() const{
    if (current_animation_) {
        AnimationFrame *frame = current_animation_->at(current_frame_);
        if (frame)
            return frame->frame();
        else return 0;
    }
    else return default_frame_;
}

const graphic::Modifier* AnimationManager::get_current_modifier() const {
    return current_animation_
            ? current_animation_->at(current_frame_)->modifier()
            : NULL;
}

void AnimationManager::Select(const std::string& name) {
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
    observers_.push_back(observer);
}

void AnimationManager::AddTickFunction(std::tr1::function<void (void)> tick) {
    ticks_.push_back(tick);
}

void AnimationManager::NotifyAllObservers() {
    for (int i = 0; i < (int)observers_.size(); ++i) {
        observers_[i]->Tick();
    }
    for(std::vector< std::tr1::function<void (void)> >::iterator it = ticks_.begin();
        it != ticks_.end(); ++it)
        (*it)();
}

} /* namespace action*/

} /* namespace ugdk */




