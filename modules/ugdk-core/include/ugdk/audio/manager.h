#ifndef UGDK_AUDIO_MANAGER_H_
#define UGDK_AUDIO_MANAGER_H_

#include <string>
#include <map>
#include <ugdk/audio/music.h>
#include <ugdk/audio/sample.h>

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

    /// Loads an audio sample.
    /**@param filepath The path to the audio sample.
     */
    std::shared_ptr<Sample> LoadSample(const std::string& filepath);

    /// Loads a music.
    /**@param filepath The path to the music file.
     */
    std::shared_ptr<Music> LoadMusic(const std::string& filepath);

    /// Getter for the music that is currently playing.
    /** @return nullptr is no music is currently playing, a
        shared_ptr<Music> otherwise. */
    std::shared_ptr<Music> CurrentMusic() const;

  private:
    static const int NUM_CHANNELS = 16;
    std::map<std::string, std::shared_ptr<Sample>> sample_data_;
    std::map<std::string, std::shared_ptr<Music>> music_data_;

    void ReleaseSamples();
    void ReleaseMusics();
};

} // namespace audio
} // namespace ugdk

#endif // UGDK_AUDIO_MANAGER_H_
