
#include <ugdk/system/config.h>
#include <ugdk/audio/source.h>
#include <ugdk/audio/sampler.h>

#include <queue>
#include <limits>
#include <iostream>

namespace ugdk {
namespace audio {

Source::Source()
    : volume_(1.0)
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

void Source::set_sampler(std::shared_ptr<Sampler> sampler_ptr) {
    current_sampler_ptr = sampler_ptr;
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
    if (current_sampler_ptr)
        current_sampler_ptr->Rewind();
}

bool Source::is_playing() const {
    return is_playing_;
}

void Source::set_volume(double vol) {
    volume_ = std::min(1.0, std::max(0.0, vol));
}

double Source::volume() const {
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
            ALuint buffer = current_sampler_ptr->Sample();
            buffers_.emplace(buffer);
            alGetError();
            alSourceQueueBuffers(name_, 1, &buffer);
            if (alGetError() != AL_NO_ERROR) {
                printf("- Error pushing to source !!\n");
                exit(1);
            }
        }
    }
}

} // namespace audio
} // namespace ugdk
