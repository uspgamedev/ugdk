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

template <typename T, bool stereo>
struct Format {}

template <>
struct Format<I8, false> {
    static ALenum value = ALenum::AL_FORMAT_MONO8;
}

template <>
struct Format<I8, true> {
    static ALenum stereo = ALenum::AL_FORMAT_STEREO8;
}

template <>
struct Format<I16, false> {
    static ALenum value = ALenum::AL_FORMAT_MONO16;
}

template <>
struct Format<I16, true> {
    static ALenum stereo = ALenum::AL_FORMAT_STEREO16;
}

class Sampler {
  public:
    virtual ALuint GetSample() = 0;
    virtual void Rewind() = 0;
}

template<typename T>
class ProceduralSampler : public Sampler {
  public:
    ~ProceduralSampler();
    virtual ALuint GetSample();
    void Rewind();
  private:
    Sampler();
    Sampler(ALsizei size, bool stereo, ALsizei freq,
            const std::function<double(I32)>& gen_func);
    const std::function<double(I32)>& gen_func_;
    std::vector<T> ALbuffer_;
    std::vector<double> buffer_;
    I32 offset_;
    ALsizei freq_;
    I64 size_;
    bool stereo_;
    static constexpr int DEFAULT_SIZE = 4096;

    friend class Manager;
};

} // namespace audio
} // namespace ugdk

#endif // UGDK_AUDIO_SAMPLER_H_
