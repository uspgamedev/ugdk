#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "animation.h"
#include "observer.h"
#include "animationset.h"
namespace ugdk {

#define ANIMATION_BUFFER_SIZE 256

/*
Animation::Animation(float fps, ...) {
    int i = 0;
    static int tmp_frame_list[ANIMATION_BUFFER_SIZE];
    va_list arg_list;
    frames_ = NULL;

    va_start(arg_list, fps);
    tmp_frame_list[0] = 0;
    while (1) {
        int next_arg = va_arg(arg_list, int);
        if (next_arg == -1) { // Indica fim da lista de argumentos
            break;
        }
        tmp_frame_list[i++] = next_arg;
    }

    CopyFrameList(tmp_frame_list, i);
    period_ = 1.0/fps;
    elapsed_time_ = 0;
    current_frame_ = 0;
}
*/

Animation::Animation(float fps, AnimationSet *set)
    : period_(1.0f/fps), frames_(NULL), animation_set_(set),
      current_frame_(0), default_frame_(0) {}


Animation::~Animation() {}

/*
void Animation::set_framelist(int frame1, ...) {
    int i = 1;
    static int tmp_frame_list[ANIMATION_BUFFER_SIZE];
    va_list arg_list;

    va_start(arg_list, frame1);
    tmp_frame_list[0] = frame1;
    while (1) {
        int next_arg = va_arg(arg_list, int);
        if (next_arg == -1) { // Indica fim da lista de argumentos
            break;
        }
        tmp_frame_list[i++] = next_arg;
    }

    CopyFrameList(tmp_frame_list, i);
}


void Animation::CopyFrameList(int frame_list[], int n_frames) {
    if(frames_ != NULL)
        delete[] frames_;
    n_frames_ = n_frames;
    frames_ = new int[n_frames];
    memcpy(frames_, frame_list, n_frames_ * sizeof(int));
}
*/

int Animation::GetFrame() {
    if (frames_) {
        AnimationFrame *frame = frames_->at(current_frame_);
        if (frame)
            return frame->frame();
        else return 0;
    }
    else return default_frame_;
}

void Animation::Select(std::string name) {
    if (animation_set_) {
        FrameSequence *new_frames = animation_set_->Search(name);
        if (frames_ != new_frames) {
            frames_ = new_frames;
            current_frame_ = 0;
            elapsed_time_ = 0;
        }
    }
}

void Animation::Select(int index) {
    if (animation_set_) {
        FrameSequence *new_frames = animation_set_->Get(index);
        if (frames_ != new_frames) {
            frames_ = new_frames;
            current_frame_ = 0;
            elapsed_time_ = 0;
        }
    }
}

void Animation::Update(float delta_t) {
    if (!frames_) return;
    elapsed_time_ += delta_t;
    if (elapsed_time_ >= period_) {
        current_frame_ = (current_frame_ + 1) % frames_->size();
        if (current_frame_ == 0) NotifyAllObservers();
        elapsed_time_ -= period_;
    }
}

void Animation::AddObserver(Observer* observer) {
    observers.push_back(observer);
}

void Animation::NotifyAllObservers() {
    for (int i = 0; i < (int)observers.size(); ++i) {
        observers[i]->Tick();
    }
}
}





