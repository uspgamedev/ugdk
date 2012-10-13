#ifndef HORUSEYE_FRAMEWORK_SAMPLE_H_
#define HORUSEYE_FRAMEWORK_SAMPLE_H_

#include <string>
#include "SDL_mixer.h"

#include <ugdk/audio.h>

namespace ugdk {
namespace audio {

/**@class Sample
 * Note: It isn't possible to instantiate a Sample directly.
 * In order to play a sound, you must call LoadSample() from AudioManager.
 * All memory management is done my the AudioManager.
 * @see AudioManager
 */
class Sample {
  public:
    /// Plays the sound.
    void Play();

    /// Plays this sample until told to stop.
    void PlayForever();

    /// Plays the sound the given number of times.
    void Play(int loops);

    /// Stops playing the sound.
    void Stop();

    /// Returns whether the sound is playing or not.
    bool IsPlaying();

    /// Sets the volume.
    /** Sets the volume between 0 and 1. 0 is silent, 1 is the max volume.
      * @param vol The volume. 
     */
    void SetVolume(double vol);
    
    /// Returns the sound's volume.
    /** @return The actual volume, 0.0 <= volume <= 1.0 */
    double Volume(); 

  private:
    Sample(const std::string& filepath);
    ~Sample();
    Mix_Chunk *data_;
    int channel_;
    double volume_;

  friend class AudioManager;
};

} // namespace audio
} // namespace ugdk

#endif
