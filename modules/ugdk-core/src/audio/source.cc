#include <ugdk/system/config.h>
#include <ugdk/audio.h>

#include "AL.h"

#include <queue>

namespace ugdk {
namespace audio {

Source::Source()
    : volume_(1.0)
    , queue_(10)
    , num_samplers_(0)
    , curr_sampler_(0)
    , buffers_(4)
{
    alGenSources(1, &name_);
}

Source::~Source() {}

bool Source::QueueSampler(Sampler *s) {
    if (s != nullptr) {
        queue_.emplace(s);
        num_samplers_++;
    }
}

Sampler Source::DequeueSampler() {
    if (!queue_.empty()) {
        queue_.pop();
        num_samplers_--;
    }
}

void Source::ClearQueue() {
    while (!queue_.empty())
        queue_.pop();
}

void Source::Play() {
    if (!is_playing_) {
        alSourcePlay(_name);
        is_playing_ = true;
    }
}

void Source::Pause() {
    if (is_playing_) {
        alSourcePause(_name);
        is_playing_ = false;
    }
}

void Source::Stop() {
    Rewind();
    alSourceStop(_name);
}

void Source::Rewind() {
    if (queue_.size() != 0) {
        curr_sampler_.rewind();
        curr_sampler_ = queue_[0];
    }
}

bool Source::IsPlaying() const {
    return is_playing_;
}

void Source::SetVolume(double vol) {
    volume_ = std::min(1.0, std::max(0.0, vol));
}

double Source::Volume() const {
    return volume_;
}

void Source::Update() {
    if (is_playing_) {
        ALint proc_buffers = 0;
        alGetSourcei(name_, AL_BUFFERS_PROCESSED, &proc_buffers);
        if (proc_buffers >= 1) {
            for (int i = 0; i < proc_buffers; i++) {
                buffers_.pop();
                alSourceUnqueueBuffers(name_, 1, &(buffers_.front()));
            }
        }
        while (buffers_.size() < 4) {
            ALuint b = queue_[curr_sampler_].GetSample();
            if (b == -1)
                curr_sampler_ = (curr_sampler_ + 1)%num_samplers_;
            else {
                buffers_.emplace(b);
                alSourceQueueBuffers(name_, 1, &b);
            }
        }
    }
}
