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
class Sample {
  public:
    void Play(); // toca o som
    void Play(int loops); // toca o som, repetindo loops vezes
    void Stop(); // para o som
    bool IsPlaying(); // o som esta tocando?

    void SetVolume(float vol); // 0.0f (quiet) <= vol <= 1.0f (loud)
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
