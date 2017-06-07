#ifndef HORUSEYE_FRAMEWORK_MUSIC_H_
#define HORUSEYE_FRAMEWORK_MUSIC_H_

#include <string>
#include "SDL_mixer.h"

#include <ugdk/audio.h>
#include <ugdk/filesystem.h>
#include <memory>

namespace ugdk {
namespace audio {

/**@class Music
 *Note: This class cannot be directly instantiated. Please use AudioManager's LoadMusic.
 *@see AudioManager
 */
class Music {
  public:
    /// Plays this music once.
    void Play();

    /// Plays this music until told to stop.
    void PlayForever();

    /// Plays this music the given number of times.
    void Play(int loops);

    /// Stops playing this music.
    void Stop();

    /// Return whether this music is playing or not.
    bool IsPlaying() const;

    /// Pauses the music, allowing it to be resumed from the same point.
    void Pause();

    /// Unpauses the music if it's the last played music.
    void Unpause();

    /// Return whether this can be unpaused.
    bool IsPaused() const;

    /// Sets the volume. 0.0 is quiet, 1.0 is the full volume.
    /** @param vol 0.0 (quiet) <= vol <= 1.0 (loud) */
    void SetVolume(double vol);

    /// Returns this music's current volume, between 0.0 and 1.0.
    double Volume();

  private:
    Music(const std::string& filepath);
    ~Music();
    Mix_Music *data_;
    double volume_;
    std::unique_ptr<ugdk::filesystem::File> file_;

    static void UpdateVolume(double vol);
    static void MusicDone();
    static std::shared_ptr<Music> playing_music_;

    friend class Manager;
};

} // namespace audio
} // namespace ugdk

#endif
