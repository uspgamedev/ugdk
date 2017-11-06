#ifndef UGDK_AUDIO_MANAGER_H_
#define UGDK_AUDIO_MANAGER_H_

#include <string>
#include <map>
#include <functional>

#include <ugdk/audio/music.h>
#include <ugdk/audio/sample.h>

#include <ugdk/audio/source.h>
#include <ugdk/audio/sampler.h>

namespace ugdk {
namespace audio {

enum class AudioFormat {
    MONO8, MONO16, STEREO8, STEREO16
};

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

    /// Loads an audio sample.
    /**@param filepath The path to the audio sample.
     */
    std::shared_ptr<Sample> LoadSample(const std::string& filepath);

    //std::shared_ptr<Sampler> LoadSampler(const std::string& filepath);
    std::shared_ptr<Sampler> LoadSampler(const std::string& name, ALsizei size, AudioFormat form,
                                         ALsizei freq, const std::function<void(U64)>& gen_func);

    /// Loads a music.
    /**@param filepath The path to the music file.
     */
    std::shared_ptr<Music> LoadMusic(const std::string& filepath);

    std::shared_ptr<Source> LoadSource();

    /// Getter for the music that is currently playing.
    /** @return nullptr is no music is currently playing, a
        shared_ptr<Music> otherwise. */
    std::shared_ptr<Music> CurrentMusic() const;

  private:
    static const int NUM_CHANNELS = 16;
    std::map<std::string, std::shared_ptr<Sample>> sample_data_;
    std::map<std::string, std::shared_ptr<Music>> music_data_;

    std::map<std::string, std::shared_ptr<Source>> souce_data_;
    std::map<std::string, std::shared_ptr<Sampler>> sampler_data_;

    void ReleaseSamples();
    void ReleaseMusics();

    void ReleaseSources();
    void ReleaseSamplers();
};

} // namespace audio
} // namespace ugdk

#endif // UGDK_AUDIO_MANAGER_H_
