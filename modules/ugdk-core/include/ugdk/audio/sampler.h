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

class Sampler {
  public:
    ~Sampler();
    virtual ALuint GetSample();
    void Rewind();
  private:
    Sampler();
    Sampler(ALsizei size, AudioFormat form, ALsizei freq,
            const std::function<void(U64)>& gen_func);
    const std::function<void(U64)>& gen_func_;
    std::vector<char> ALbuffer_;
    std::vector<float> buffer_;
    ALsizei offset_;
    ALsizei freq_;
    U64 size_;
    AudioFormat form_;
    static constexpr int DEFAULT_SIZE = 4096;

    friend class Manager;
};

} // namespace audio
} // namespace ugdk

#endif // UGDK_AUDIO_SAMPLER_H_
