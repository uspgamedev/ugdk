#ifndef HORUSEYE_FRAMEWORK_SAMPLE_H_
#define HORUSEYE_FRAMEWORK_SAMPLE_H_

// Nao e' possivel instanciar um Sample
// diretamente.
//
// Para tocar um som, voce deve chamar
// o metodo LoadSample() do AudioManager
// (ele ja faz todo o gerenciamento de
// memoria para voce)

#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

namespace ugdk {

class AudioManager;

// Efeito sonoro
/**@class Sample
 * Note: It isn't possible to instantiate a Sample directly.
 * In order to play a sound, you must call LoadSample() from AudioManager.
 * All memory management is done my the AudioManager.
 * @see AudioManager
 */
class Sample {
  public:
    ///Plays the sound.
    void Play(); // toca o som
    ///Plays the sound the given number of times.
    void Play(int loops); // toca o som, repetindo loops vezes
    ///Stops playing the sound.
    void Stop(); // para o som
    ///Returns whether the sound is playing or not.
    bool IsPlaying(); // o som esta tocando?

    ///Sets the volume.
    /**Sets the volume between 0 and 1. 0 is silent, 1 is the max volume.
     */
    void SetVolume(float vol); // 0.0f (quiet) <= vol <= 1.0f (loud)
    ///Returns the sound's volume.
    float Volume(); // obtem o volume, 0.0f <= volume <= 1.0f

  private:
    Sample(const std::string& filepath);
    ~Sample();
    Mix_Chunk *data_;
    int channel_;
    float volume_;

  friend class AudioManager;
};

}  // namespace framework

#endif
