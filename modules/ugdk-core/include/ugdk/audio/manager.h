#ifndef UGDK_AUDIO_MANAGER_H_
#define UGDK_AUDIO_MANAGER_H_

#include <ugdk/audio/sampler.h>
#include <ugdk/audio/source.h>
#include <ugdk/audio/sampleframe.h>
#include <ugdk/structure/types.h>

#include UGDK_OPENAL_DIR(al.h)
#include UGDK_OPENAL_DIR(alc.h)

#include <string>
#include <map>
#include <functional>

namespace ugdk {
namespace audio {

class Manager {
  public:
    Manager();
    ~Manager();

    /// Initializes audio channels.
    /** @return True if successful, false otherwise.
     */
    bool Initialize();

    /// Frees all audio samples and music, and closes the audio channels.
    void Release();

    /// Logical update. Currently does nothing.
    void Update();

    //std::shared_ptr<Sampler> LoadSampler(const std::string& filepath);
    std::shared_ptr<Sampler> LoadSampler(const std::string& name, U64 sample_rate, U8 num_channels,
                                         const Sampler::Processor& processor);

    std::shared_ptr<Source> LoadSource(const std::string& name);

  private:
    static const int NUM_CHANNELS = 16;

    std::map<std::string, std::shared_ptr<Source>> source_data_;
    std::map<std::string, std::shared_ptr<Sampler>> sampler_data_;

    ALCdevice *device_;

    void ReleaseSources();
    void ReleaseSamplers();
};

} // namespace audio
} // namespace ugdk

#endif // UGDK_AUDIO_MANAGER_H_
