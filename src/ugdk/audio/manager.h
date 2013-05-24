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
    ~Manager();

    /// Initializes audio channels.
    /** @return True if successful, false otherwise.
     */
    bool Initialize();

    /// Frees all audio samples and music, and closes the audio channels.
    /** @return True if successfull, false otherwise.
     */
    bool Release();

    /// Logical update. Currently does nothing.
    void Update();

    /// Loads an audio sample.
    /**@param filepath The path to the audio sample.
     */
    Sample* LoadSample(const std::string& filepath);

    /// Loads a music.
    /**@param filepath The path to the music file.
     */ 
    Music* LoadMusic(const std::string& filepath);

    /// Getter for the music that is currently playing.
    /** @return nullptr is no music is currently playing, a
        Music* otherwise. */
    Music* CurrentMusic() const;

  private:
    static const int NUM_CHANNELS = 16;
    std::map<std::string, Sample*> sample_data_;
    std::map<std::string, Music*> music_data_;

    Manager();

    void ReleaseSamples();
    void ReleaseMusics();

    friend class Engine;
};

} // namespace audio
} // namespace ugdk

#endif // UGDK_AUDIO_MANAGER_H_
