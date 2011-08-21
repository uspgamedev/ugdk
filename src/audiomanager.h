#ifndef HORUSEYE_FRAMEWORK_AUDIOMANAGER_H_
#define HORUSEYE_FRAMEWORK_AUDIOMANAGER_H_

#include <string>
#include <map>
#include "music.h"
#include "sample.h"

#define AUDIO_MANAGER() ugdk::Engine::reference()->audio_manager()

namespace ugdk {

// Gerenciador de audio
class AudioManager {
  public:
    AudioManager();
    ~AudioManager();

    bool Initialize();
    bool Release();
    void Update();

    Sample* LoadSample(const std::string& filepath);
    Music* LoadMusic(const std::string& filepath);

  private:
    std::map<std::string, Sample*> sample_data_;
    std::map<std::string, Music*> music_data_;
    static const int NUM_CHANNELS = 16;

    void ReleaseSamples();
    void ReleaseMusics();
};

}  // namespace framework

#endif
