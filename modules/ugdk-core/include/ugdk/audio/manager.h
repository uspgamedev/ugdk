#ifndef UGDK_AUDIO_MANAGER_H_
#define UGDK_AUDIO_MANAGER_H_

#include <ugdk/system/config.h>
#include <ugdk/audio/sampler.h>
#include <ugdk/audio/track.h>
#include <ugdk/audio/sampleframe.h>
#include <ugdk/structure/types.h>

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

    std::shared_ptr<Track> LoadTrack(const std::string& name);

  private:
    struct Backend;

    static const int NUM_CHANNELS = 16;

    std::map<std::string, std::shared_ptr<Track>> track_data_;
    std::map<std::string, std::shared_ptr<Sampler>> sampler_data_;

    std::unique_ptr<Backend> backend_;

    void ReleaseTracks();
    void ReleaseSamplers();
};

} // namespace audio
} // namespace ugdk

#endif // UGDK_AUDIO_MANAGER_H_
