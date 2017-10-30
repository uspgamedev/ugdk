#include <ugdk/audio/sampler.h>

#include "AL.h"

#include <functional>
#include <vector>

Sampler::Sampler() {}

Sampler::Sampler(ALsizei size, AudioFormat form, ALsizei freq, const std::function<void(unsigned int)>& gen_func)
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
    if (offset_ >= ALbuffer.size()) {
        offset_ = 0;
        return -1;
    }
    alDeleteBuffers(1, &name_);
    size_t i;
    for (i = 0; i < DEFAULT_SIZE && offset_ + i < ALbuffer.size(); i++)
        buffer_[i] = gen_func_(offset_ + i);
    offset_ += i;
    alGenBuffers(1, &name_);
    alBufferData(name_, form_, (ALvoid*)(ALbuffer_.data()), i, freq_);
    return name_;
}

void Sampler::Rewind() {
    offset_ = 0
}
