
#ifndef UGDK_AUDIO_SAMPLER_H_
#define UGDK_AUDIO_SAMPLER_H_

#include <ugdk/system/config.h>
#include <ugdk/audio/manager.h>
#include <ugdk/audio/sampledata.h>
#include <ugdk/structure/types.h>

#include UGDK_OPENAL_DIR(al.h)

#include <functional>
#include <vector>

namespace ugdk {
namespace audio {

template <typename T>
struct Format {};

template <>
struct Format<I8> {
    static constexpr ALenum mono = AL_FORMAT_MONO8;
    static constexpr ALenum stereo = AL_FORMAT_STEREO8;
};

template <>
struct Format<I16> {
    static constexpr ALenum mono = AL_FORMAT_MONO16;
    static constexpr ALenum stereo = AL_FORMAT_STEREO16;
};

class Sampler {
  public:
    using Processor = std::function<void(SampleFrame*)>;

    void Sample(SampleData *sample_data);
  private:
    Sampler(U64 sample_rate, U8 num_channels, const Processor& process);

    const Processor process_;
    U64 sample_rate_;
    U64 offset_;
    U8  num_channels_;

    friend class Manager;
};

} // namespace audio
} // namespace ugdk

#endif // UGDK_AUDIO_SAMPLER_H_
