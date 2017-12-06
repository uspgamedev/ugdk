#include <ugdk/audio/sampler.h>
#include <ugdk/audio/manager.h>

#include <functional>
#include <vector>
#include <limits>
#include <iostream>

namespace ugdk {
namespace audio {

Sampler::Sampler()
    : gen_func_([](I32 n) { return 0.0; })
    , ALbuffer_(DEFAULT_SIZE)
    , buffer_(DEFAULT_SIZE)
    , offset_(0)
    , freq_(0)
    , size_(0)
    , form_(AudioFormat::MONO8)
{}

Sampler::Sampler(ALsizei size, AudioFormat form, ALsizei freq, const std::function<double(I32)>& gen_func)
    : gen_func_(gen_func)
    , ALbuffer_(size)
    , buffer_(size)
    , offset_(0)
    , freq_(freq)
    , size_(size)
    , form_(form)
{}

Sampler::~Sampler() {}

ALuint Sampler::GetSample() {
    ALuint name = 0;
    I16 I16_MAX = std::numeric_limits<I16>::max();
    I8 I8_MAX = std::numeric_limits<I8>::max();
    I16 *reint_ALbuffer_ = reinterpret_cast<I16*>(ALbuffer_.data());
    I32 i = 0;
    I32 factor = 1;
    if (offset_ >= size_) {
        offset_ = 0;
        return std::numeric_limits<ALuint>::max();
    }
    switch (form_) {
      case AudioFormat::MONO8:
        for (i = 0; i < DEFAULT_SIZE && offset_ + i < size_; i++) {
            buffer_[i] = gen_func_(offset_ + i);
            ALbuffer_[i] = static_cast<U8>(I8_MAX*buffer_[i]);
        }
        offset_ += i;
        break;
      case AudioFormat::MONO16:
        for (i = 0; i < DEFAULT_SIZE/2 && offset_ + i < size_; i++) {
            buffer_[i] = gen_func_(offset_ + i);
            reint_ALbuffer_[i] = static_cast<U16>(I16_MAX*buffer_[i]);
        }
        factor = 2;
        offset_ += i;
        break;
      case AudioFormat::STEREO8:
        for (i = 0; i < DEFAULT_SIZE && offset_ + i/2 < size_; i += 2) {
            buffer_[i] = gen_func_(offset_ + i/2);
            ALbuffer_[i] = static_cast<U8>(I8_MAX*buffer_[i]);
            buffer_[i+1] = buffer_[i];
            ALbuffer_[i+1] = ALbuffer_[i];
        }
        offset_ += i/2;
        break;
      case AudioFormat::STEREO16:
        for (i = 0; i < DEFAULT_SIZE/2 && offset_ + i/2 < size_; i += 2) {
            buffer_[i] = gen_func_(offset_ + i/2);
            reint_ALbuffer_[i] = static_cast<U16>(I16_MAX*buffer_[i]);
            buffer_[i+1] = buffer_[i];
            reint_ALbuffer_[i+1] = reint_ALbuffer_[i];
        }
        factor = 2;
        offset_ += i/2;
        break;
    }

    alGetError();
    alGenBuffers(1, &name);

    if (alGetError() != AL_NO_ERROR) {
        printf("- Error creating buffers !!\n");
        exit(1);
    }

    alBufferData(name, static_cast<ALenum>(form_), static_cast<ALvoid*>(ALbuffer_.data()),
                 static_cast<ALsizei>(factor*i), freq_);

    return name;
}

void Sampler::Rewind() {
    offset_ = 0;
}

} // namespace audio
} // namespace ugdk
