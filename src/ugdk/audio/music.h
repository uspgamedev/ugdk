#ifndef HORUSEYE_FRAMEWORK_MUSIC_H_
#define HORUSEYE_FRAMEWORK_MUSIC_H_

#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

namespace ugdk {

class AudioManager;

// Musica
/**@class Music
 *Note: This class cannot be directly instantiated. Please use AudioManager's LoadMusic.
 *@see AudioManager
 */
class Music {
  public:
    ///Plays the music once.
    void Play(); // toca musica uma vez
    ///Plays the music the given number of times.
    void Play(int loops); // toca musica loops vezes
    ///Plays the music until told to stop.
    void PlayForever(); // toca musica infinitas vezes
    ///Stops playing hte music.
    void Stop(); // para musica
    ///Return whether the music is playing or not.
    bool IsPlaying(); // esta musica esta' tocando?

    ///Sets the volume. 0 is quiet, 1 is the full volume.
    void SetVolume(float vol); // 0.0f (quiet) <= vol <= 1.0f (loud)
    ///Returns the music's curretn volume, between 0 and 1.
    float Volume(); // obtem volume, 0.0f <= volume <= 1.0f

  private:
    Music(const std::string& filepath);
    ~Music();
    Mix_Music *data_;
    float volume_;

    static void UpdateVolume(float vol);
    static void MusicDone();
    static Mix_Music *playing_music_;

  friend class AudioManager;
};

}  // namespace framework

#endif
