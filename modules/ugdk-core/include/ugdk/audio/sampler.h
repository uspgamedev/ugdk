#ifndef UGDK_AUDIO_SAMPLER_H_
#define UGDK_AUDIO_SAMPLER_H_

#include <ugdk/system/config.h>
#include "AL.h"

namespace ugdk {
namespace audio {

class Sampler {
  public:
    GetSample();
    void Rewind();
  private:
    Sampler();
    ~Sampler();
    double t_;
    double tf_;

};

} // namespace audio
} // namespace ugdk

#endif // UGDK_AUDIO_SAMPLER_H_
