#ifndef HORUSEYE_FRAMEWORK_MUSIC_H_
#define HORUSEYE_FRAMEWORK_MUSIC_H_

#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

namespace ugdk {

class AudioManager;

// Musica
class Music {
  public:
    void Play(); // toca musica uma vez
    void Play(int loops); // toca musica loops vezes
    void PlayForever(); // toca musica infinitas vezes
    void Stop(); // para musica
    bool IsPlaying(); // esta musica esta' tocando?

    void SetVolume(float vol); // 0.0f (quiet) <= vol <= 1.0f (loud)
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
