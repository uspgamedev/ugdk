#ifndef UGDK_AUDIO_SOURCE_H_
#define UGDK_AUDIO_SOURCE_H_
#include <ugdk/system/config.h>
#include <ugdk/structure/types.h>
#include <ugdk/audio.h>

#include UGDK_OPENAL_DIR(al.h)

#include <vector>
#include <queue>

namespace ugdk {
namespace audio {

class Source {
  public:
    Source();
    ~Source();
    bool QueueSampler(Sampler s);
    Sampler DequeueSampler();
    void ClearQueue();
    void Play();
    void Pause();
    void Stop();
    void Rewind();
    bool IsPlaying() const;
    void SetVolume(double vol);
    double Volume() const;
  private:
    void Update();
    std::vector<Sampler*> queue_;
    std::queue<ALuint> buffers_;
    size_t curr_sampler_;
    size_t num_samplers_;
    double volume_;
    bool is_playing_;
    ALuint name_;

    friend class Manager;
};

} // namespace audio
} // namespace ugdk

#endif //UGDK_AUDIO_SOURCE_H_
