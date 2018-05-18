
#include <ugdk/audio/sampler.h>

#include <ugdk/audio/manager.h>

#include <functional>
#include <vector>
#include <limits>
#include <iostream>

namespace ugdk {
namespace audio {

Sampler::Sampler(U64 sample_rate, U8 num_channels, const Processor& process)
    : process_(process), sample_rate_(sample_rate), offset_(0u), num_channels_(num_channels) {}

void Sampler::Sample(SampleData *sample_data) {
    for (U64 i = 0; i < sample_data->size(); i++) {
        SampleFrame frame(offset_ + i, sample_rate_, num_channels_);
        process_(&frame);
        for (U8 j = 0; j < num_channels_; j++)
            sample_data->set_channel_sample(i, j, frame.channel(j));
    }
    offset_ += sample_data->size();
    /*
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
    */
}

} // namespace audio
} // namespace ugdk
