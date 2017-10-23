#ifndef UGDK_AUDIO_PSAMPLER_H_
#define UGDK_AUDIO_PSAMPLER_H_

#include <ugdk/system/config.h>
#include <ugdk/audio/manager.h>

#include "AL.h"

#include <functional>

namespace ugdk {
namespace audio {

class PSampler : private Sampler {
  public:
    PSampler(unsigned long long size, AudioFormat form, const std::function<void(unsigned long long)>& gen_func);
    ~PSampler();
  private:
    ALuint name_;
    unsigned long long size_;
    AudioFormat form_;
    const std::function<void(unsigned long long)>& gen_func_;
};

} // namespace audio
} // namespace ugdk

#endif // UGDK_AUDIO_PSAMPLER_H_
