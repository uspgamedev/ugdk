
#ifndef UGDK_AUDIO_TRACK_H_
#define UGDK_AUDIO_TRACK_H_

#include <ugdk/system/config.h>
#include <ugdk/structure/types.h>
#include <ugdk/audio/sampler.h>

#include <vector>
#include <memory>
#include <queue>

namespace ugdk {
namespace audio {

class Sampler;

class Track {
  public:
    void set_sampler(std::shared_ptr<Sampler>);
    void Play();
    void Pause();
    void Stop();
    bool is_playing() const;
    void set_volume(double vol);
    double volume() const;
  protected:
    Track();
    virtual ~Track();
    virtual void Update() = 0;
  private:
    std::shared_ptr<Sampler> current_sampler_ptr;
    friend class Manager;
};

class SpatialTrack : public Track {
  public:
    SpatialTrack();
};

} // namespace audio
} // namespace ugdk

#endif //UGDK_AUDIO_TRACK_H_
