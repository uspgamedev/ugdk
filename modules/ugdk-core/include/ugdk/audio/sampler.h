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
    virtual void GetSample();
    void Rewind();
  private:
    Sampler();
    Sampler(ALsizei size, AudioFormat form, const std::function<void(unsigned long long)>& gen_func);
    ALuint name_;
    ALsizei offset_;
    vector<char> ALbuffer_;
    vector<float> buffer_;
    ALsizei size_;
    AudioFormat form_;
    const std::function<void(unsigned long long)>& gen_func_;
};

} // namespace audio
} // namespace ugdk

#endif // UGDK_AUDIO_SAMPLER_H_
