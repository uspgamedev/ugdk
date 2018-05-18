
#ifndef UGDK_AUDIO_SOURCE_H_
#define UGDK_AUDIO_SOURCE_H_

#include <ugdk/system/config.h>
#include <ugdk/structure/types.h>
#include <ugdk/audio/sampler.h>

#include UGDK_OPENAL_DIR(al.h)

#include <vector>
#include <memory>
#include <queue>

namespace ugdk {
namespace audio {

class Sampler;

class Source {
  public:
    Source();
    ~Source();
    void set_sampler(std::shared_ptr<Sampler>);
    void Play();
    void Pause();
    void Stop();
    bool is_playing() const;
    void set_volume(double vol);
    double volume() const;
  private:
    void Update();
    std::shared_ptr<Sampler> current_sampler_ptr;
    std::queue<ALuint> buffers_;
    double volume_;
    bool is_playing_;
    ALuint name_;

    friend class Manager;
};

} // namespace audio
} // namespace ugdk

#endif //UGDK_AUDIO_SOURCE_H_
