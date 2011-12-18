#ifndef HORUSEYE_FRAMEWORK_AUDIOMANAGER_H_
#define HORUSEYE_FRAMEWORK_AUDIOMANAGER_H_

#include <string>
#include <map>
#include <ugdk/audio/music.h>
#include <ugdk/audio/sample.h>

#define AUDIO_MANAGER() ugdk::Engine::reference()->audio_manager()

namespace ugdk {

// Gerenciador de audio
class AudioManager {
  public:
    AudioManager();
    ~AudioManager();

    ///Initializes audio channels.
    /** @return True if successful, false otherwise.
     */
    bool Initialize();

    ///Frees all audio samples and music, and closes the audio channels.
    /** @return True if successfull, false otherwise.
     */
    bool Release();

    ///Logical update. Currently does nothing.
    void Update();

    ///Loads an audio sample.
    /**@param filepath The path to the audio sample.
     */
    Sample* LoadSample(const std::string& filepath);

    ///Loads a music.
    /**@param filepath The path to the music file.
     */ 
    Music* LoadMusic(const std::string& filepath);

    /// Getter for the music that is currently playing.
    /** @return NULL is no music is currently playing, a
        Music* otherwise. */
    Music* CurrentMusic() const;

  private:
    std::map<std::string, Sample*> sample_data_;
    std::map<std::string, Music*> music_data_;
    static const int NUM_CHANNELS = 16;

    void ReleaseSamples();
    void ReleaseMusics();
};

}  // namespace framework

#endif
