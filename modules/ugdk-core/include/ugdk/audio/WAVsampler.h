#ifndef UGDK_AUDIO_SAMPLER_H_
#define UGDK_AUDIO_SAMPLER_H_

#include <ugdk/system/config.h>
#include <ugdk/audio/manager.h>
#include <ugdk/audio/WaveDecoder.h>

#include UGDK_OPENAL_DIR(AL.h)
#include "Wuff.h"

#include <string>

namespace ugdk {
namespace audio {

class WAVSampler : private Sampler {
  public:
    override void GetSample();
  private:
    WAVSampler(const std::string& filename, ALsizei size, AudioFormat form);
    WaveDecoder decoder_;
};

} // namespace audio
} // namespace ugdk

#endif // UGDK_AUDIO_SAMPLER_H_
