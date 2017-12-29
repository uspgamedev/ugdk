#include <ugdk/system/config.h>
#include <ugdk/audio.h>
#include <ugdk/audio/sampler.h>

#include <queue>
#include <limits>
#include <iostream>

namespace ugdk {
namespace audio {

Source::Source()
    : curr_sampler_(0)
    , num_samplers_(0)
    , volume_(1.0)
    , is_playing_(false)
{
    alGetError();
    alGenSources(1, &name_);
    if (alGetError() != AL_NO_ERROR) {
        printf("- Error creating source !!\n");
        exit(1);
    }
    alSourcef(name_, AL_GAIN, 1);
    alSourcef(name_, AL_PITCH, 1);
    alSource3f(name_, AL_POSITION, 0, 0, 0);
}

Source::~Source() {}

bool Source::QueueSampler(Sampler *s) {
    if (s != nullptr) {
        queue_.emplace(queue_.begin(), s);
        num_samplers_++;
        return true;
    }
    return false;
}

Sampler* Source::DequeueSampler() {
    Sampler *s = nullptr;
    if (!queue_.empty()) {
        s = queue_.back();
        queue_.pop_back();
        num_samplers_--;
    }
    return s;
}

void Source::ClearQueue() {
    while (!queue_.empty())
        queue_.pop_back();
}

void Source::Play() {
    if (!is_playing_) {
        is_playing_ = true;
        Update();
        alGetError();
        alSourcePlay(name_);
        if (alGetError() != AL_NO_ERROR) {
            printf("- Error playing source !!\n");
            exit(1);
        }
    }
}

void Source::Pause() {
    if (is_playing_) {
        alSourcePause(name_);
        is_playing_ = false;
    }
}

void Source::Stop() {
    Rewind();
    alSourceStop(name_);
}

void Source::Rewind() {
    if (queue_.size() != 0) {
        queue_[curr_sampler_]->Rewind();
        curr_sampler_ = 0;
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
                ALuint b = buffers_.back();
                buffers_.pop();
                alGetError();
                alSourceUnqueueBuffers(name_, 1, &b);
                alDeleteBuffers(1, &b);
                if (alGetError() != AL_NO_ERROR) {
                    printf("- Error poping from source !!\n");
                    exit(1);
                }
            }
        }
        while (buffers_.size() < 4) {
            ALuint b = queue_[curr_sampler_]->Sample();
            if (b == std::numeric_limits<ALuint>::max()) {
                curr_sampler_ = (curr_sampler_ + 1)%num_samplers_;
                std::cout << "Next Sampler" << std::endl;
            }
            else {
                buffers_.emplace(b);
                alGetError();
                alSourceQueueBuffers(name_, 1, &b);
                if (alGetError() != AL_NO_ERROR) {
                    printf("- Error pushing to source !!\n");
                    exit(1);
                }
            }
        }
    }
}

} // namespace audio
} // namespace ugdk
