#include <ugdk/audio/sampler.h>
#include <ugdk/audio/manager.h>

#include <functional>
#include <vector>
#include <limits>
#include <iostream>

namespace ugdk {
namespace audio {

template <typename T>
ProceduralSampler<T>::ProceduralSampler()
    : gen_func_([](I32 n) { return 0.0; })
    , ALbuffer_(DEFAULT_SIZE)
    , buffer_(DEFAULT_SIZE)
    , freq_(1)
    , offset_(0)
    , size_(0)
    , stereo_(false)
{}

template <typename T>
ProceduralSampler<T>::ProceduralSampler(ALsizei size, bool stereo, ALsizei freq,
                                     const std::function<double(I32)>& gen_func)
    : gen_func_(gen_func)
    , ALbuffer_(size)
    , buffer_(size)
    , freq_(freq)
    , offset_(0)
    , size_(size)
    , stereo_(stereo)
{}

template <typename T>
ALuint ProceduralSampler<T>::Sample() {
    ALuint name = 0;
    T T_MAX = std::numeric_limits<T>::max();
    I32 i = 0;
    if (offset_ >= size_) {
        offset_ = 0;
        return std::numeric_limits<ALuint>::max();
    }
    if (!stereo_) {
        for (i = 0; i < DEFAULT_SIZE && offset_ + i < size_; i++) {
            buffer_[i] = gen_func_(offset_ + i);
            ALbuffer_[i] = static_cast<T>(T_MAX*buffer_[i]);
        }
        offset_ += i;
    }
    else {
        for (i = 0; i < DEFAULT_SIZE && offset_ + i/2 < size_; i += 2) {
            buffer_[i] = gen_func_(offset_ + i/2);
            ALbuffer_[i] = static_cast<T>(T_MAX*buffer_[i]);
            buffer_[i+1] = buffer_[i];
            ALbuffer_[i+1] = ALbuffer_[i];
        }
        offset_ += i/2;
    }

    alGetError();
    alGenBuffers(1, &name);

    if (alGetError() != AL_NO_ERROR) {
        printf("- Error creating buffers !!\n");
        exit(1);
    }

    if (stereo_)
        alBufferData(name, Format<T>::stereo, static_cast<ALvoid*>(ALbuffer_.data()),
                     static_cast<ALsizei>(i*sizeof(T)), freq_);
    else
        alBufferData(name, Format<T>::mono, static_cast<ALvoid*>(ALbuffer_.data()),
                     static_cast<ALsizei>(i*sizeof(T)), freq_);


    return name;
}

template <typename T>
void ProceduralSampler<T>::Rewind() {
    offset_ = 0;
}

} // namespace audio
} // namespace ugdk