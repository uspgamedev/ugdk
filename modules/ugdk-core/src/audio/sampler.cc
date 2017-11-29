#include <ugdk/audio/sampler.h>
#include <ugdk/audio/manager.h>

#include <functional>
#include <vector>
#include <limits>
#include <iostream>

namespace ugdk {
namespace audio {

Sampler::Sampler()
    : gen_func_([](U32 n) { return 0.0; })
    , ALbuffer_(DEFAULT_SIZE)
    , buffer_(DEFAULT_SIZE)
    , offset_(0)
    , freq_(0)
    , size_(0)
    , form_(AudioFormat::MONO8)
{}

Sampler::Sampler(ALsizei size, AudioFormat form, ALsizei freq, const std::function<float(U32)>& gen_func)
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
    U16 U16_MAX = std::numeric_limits<U16>::max();
    U8 U8_MAX = std::numeric_limits<U8>::max();
    U16 *reint_ALbuffer_ = reinterpret_cast<U16*>(ALbuffer_.data());
    if (offset_ >= size_/((int)form_%2 + 1)) {
        offset_ = 0;
        return std::numeric_limits<ALuint>::max();
    }
    U32 i = 0;
    switch (form_) {
      case AudioFormat::MONO8:
        for (i = 0; i < DEFAULT_SIZE && offset_ + i < size_; i++) {
            buffer_[i] = gen_func_(offset_ + i);
            ALbuffer_[i] = (U8)(U8_MAX*buffer_[i]);
        }
        break;
      case AudioFormat::MONO16:
        for (i = 0; i < DEFAULT_SIZE && offset_ + i < size_/2; i++) {
            buffer_[i] = gen_func_(offset_ + i);
            reint_ALbuffer_[i] = (U16)(U16_MAX*buffer_[i]);
        }
        break;
      case AudioFormat::STEREO8:
        for (i = 0; i < DEFAULT_SIZE && offset_ + i < size_; i += 2) {
            buffer_[i] = gen_func_(offset_ + i);
            ALbuffer_[i] = (U8)(U8_MAX*buffer_[i]);
            buffer_[i+1] = buffer_[i];
            ALbuffer_[i+1] = ALbuffer_[i];
        }
        break;
      case AudioFormat::STEREO16:
        for (i = 0; i < DEFAULT_SIZE && offset_ + i < size_/2; i += 2) {
            buffer_[i] = gen_func_(offset_ + i);
            reint_ALbuffer_[i] = (U16)(U16_MAX*buffer_[i]);
            buffer_[i+1] = buffer_[i];
            reint_ALbuffer_[i+1] = reint_ALbuffer_[i];
        }
        break;
    }
    offset_ += i;

    alGetError();
    alGenBuffers(1, &name);

    if (alGetError() != AL_NO_ERROR) {
        printf("- Error creating buffers !!\n");
        exit(1);
    }

    alBufferData(name, (ALenum)form_, (ALvoid*)(ALbuffer_.data()), (ALsizei)i, freq_);

    return name;
}

void Sampler::Rewind() {
    offset_ = 0;
}

} // namespace audio
} // namespace ugdk
