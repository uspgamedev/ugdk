#include <ugdk/audio/sampler.h>
#include <ugdk/audio/manager.h>

#include <functional>
#include <vector>

namespace ugdk {
namespace audio {

Sampler::Sampler()
    : gen_func_([](ALsizei) { return 0.0; })
    , ALbuffer_(DEFAULT_SIZE)
    , buffer_(DEFAULT_SIZE)
    , offset_(0)
    , freq_(0)
    , size_(0)
    , form_(AudioFormat::MONO8)
{}

Sampler::Sampler(ALsizei size, AudioFormat form, ALsizei freq, const std::function<float(ALsizei)>& gen_func)
    : gen_func_(gen_func)
    , ALbuffer_(DEFAULT_SIZE)
    , buffer_(DEFAULT_SIZE)
    , offset_(0)
    , freq_(freq)
    , size_(size)
    , form_(form)
{}

Sampler::~Sampler() {}

ALuint Sampler::GetSample() {
    ALuint name = 0;
    if (offset_ >= ALbuffer_.size()) {
        offset_ = 0;
        return -1;
    }
    ALsizei i;
    for (i = 0; i < DEFAULT_SIZE && offset_ + i < ALbuffer_.size(); i++)
        buffer_[i] = gen_func_(offset_ + i);
        //ALbuffer_[i] =
    offset_ += i;

    alGetError();
    alGenBuffers(1, &name);

    if (alGetError() != AL_NO_ERROR) {
        printf("- Error creating buffers !!\n");
        exit(1);
    }

    alBufferData(name, (ALenum)form_, (ALvoid*)(ALbuffer_.data()), i, freq_);

    return name;
}

void Sampler::Rewind() {
    offset_ = 0;
}

} // namespace audio
} // namespace ugdk
