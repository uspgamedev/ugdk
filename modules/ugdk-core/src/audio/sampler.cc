#include <ugdk/audio/sampler.h>

#include "AL.h"

#include <functional>
#include <vector>

Sampler::Sampler() {}

Sampler::Sampler(ALsizei size, AudioFormat form, ALsizei freq, const std::function<void(U64)>& gen_func)
    : size_(size)
    , form_(form)
    , freq_(freq)
    , gen_func_(gen_func)
    , offset_(0)
    , ALbuffer_(DEFAULT_SIZE)
    , buffer_(DEFAULT_SIZE)
{}

Sampler::~Sampler() {}

virtual ALuint Sampler::GetSample() {
    ALuint name = 0;
    if (offset_ >= ALbuffer_.size()) {
        offset_ = 0;
        return -1;
    }
    ALsizei i;
    for (i = 0; i < DEFAULT_SIZE && offset_ + i < ALbuffer_.size(); i++)
        buffer_[i] = gen_func_(offset_ + i);
    offset_ += i;

    alGetError();
    alGenBuffers(1, &name);

    if (alGetError() != AL_NO_ERROR) {
        printf("- Error creating buffers !!\n");
        exit(1);
    }
    
    alBufferData(name, form_, (ALvoid*)(ALbuffer_.data()), i, freq_);

    return name;
}

void Sampler::Rewind() {
    offset_ = 0
}
