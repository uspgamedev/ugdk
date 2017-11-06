#ifndef UGDK_AUDIO_SAMPLER_H_
#define UGDK_AUDIO_SAMPLER_H_

#include <ugdk/system/config.h>
#include <ugdk/audio/manager.h>

#include "AL.h"

#include <functional>
#include <vector>

namespace ugdk {
namespace audio {

class Sampler {
  public:
    ~Sampler();
    virtual ALuint GetSample();
    void Rewind();
  private:
    Sampler();
    Sampler(ALsizei size, AudioFormat form, const std::function<void(U64)>& gen_func);
    const std::function<void(U64)>& gen_func_;
    vector<char> ALbuffer_;
    vector<float> buffer_;
    ALsizei offset_;
    ALsizei freq_;
    U64 size_;
    AudioFormat form_;
    static constexpr int DEFAULT_SIZE = 4096;
};

} // namespace audio
} // namespace ugdk

#endif // UGDK_AUDIO_SAMPLER_H_
