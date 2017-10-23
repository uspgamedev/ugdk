#ifndef UGDK_AUDIO_SOURCE_H_
#define UGDK_AUDIO_SOURCE_H_
#include <ugdk/system/config.h>
#include <ugdk/audio.h>

#include "AL.h"

#include <vector>

namespace ugdk {
namespace audio {

class Source {
  public:
    Source();
    ~Source();
    bool QueueSampler(Sampler s);
    Sampler UnqueueSampler();
    void ClearQueue();
    void Play();
    void Pause();
    void Stop();
    void Rewind();
    bool IsPlaying() const;
    bool IsPaused() const;
    void SetVolume(double vol);
    double Volume() const;
  private:
    vector<Sampler*> queue_;
    Sampler *actual_;
    double volume_;
    ALuint name_;
};

} // namespace audio
} // namespace ugdk

#endif //UGDK_AUDIO_SOURCE_H_
