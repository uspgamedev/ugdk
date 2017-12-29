#ifndef UGDK_AUDIO_SAMPLER_H_
#define UGDK_AUDIO_SAMPLER_H_

#include <ugdk/system/config.h>
#include <ugdk/audio/manager.h>
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
    virtual ALuint Sample() = 0;
    virtual void Rewind() = 0;
};

template<typename T>
class ProceduralSampler : public Sampler {
  public:
    ~ProceduralSampler() {};
    ALuint Sample();
    void Rewind();
  private:
    ProceduralSampler();
    ProceduralSampler(ALsizei size, bool stereo, ALsizei freq,
                      const std::function<double(I32)>& gen_func);
    const std::function<double(I32)>& gen_func_;
    std::vector<T> ALbuffer_;
    std::vector<double> buffer_;
    ALsizei freq_;
    I32 offset_;
    I64 size_;
    bool stereo_;
    static constexpr int DEFAULT_SIZE = 4096;

    friend class Manager;
};

template class ProceduralSampler<I8>;
template class ProceduralSampler<I16>;

} // namespace audio
} // namespace ugdk

#endif // UGDK_AUDIO_SAMPLER_H_
