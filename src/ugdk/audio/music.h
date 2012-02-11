#ifndef HORUSEYE_FRAMEWORK_MUSIC_H_
#define HORUSEYE_FRAMEWORK_MUSIC_H_

#include <string>
#include <ugdk/config/config.h>
#include "SDL.h"
#include "SDL_mixer.h"

namespace ugdk {

class AudioManager;

// Musica
/**@class Music
 *Note: This class cannot be directly instantiated. Please use AudioManager's LoadMusic.
 *@see AudioManager
 */
class Music {
  public:
    /// Plays this music once.
    void Play();

    /// Plays this music the given number of times.
    void Play(int loops);

    /// Plays this music until told to stop.
    void PlayForever();

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

    static void UpdateVolume(double vol);
    static void MusicDone();
    static Music *playing_music_;

  friend class AudioManager;
};

}  // namespace ugdk

#endif
