#ifndef UGDK_ACTION_MEDIAPLAYER_H_
#define UGDK_ACTION_MEDIAPLAYER_H_

#include <ugdk/action.h>

#include <vector>
#include <functional>

#define DEFAULT_PERIOD 0.1

namespace ugdk {
namespace action {

class MediaManager;

/*TODO
 * Represents a sprite's current animation.
 *
 * An AnimationManager object contains a sequence of information describing a sprite's visual behavior
 * (animation spreadsheet frames, position, rotation, transparency, etc) throughout a corresponding sequence
 * of the game's update frames. This per-frame information is represented by the class
 * AnimationManager::AnimationFrame, and the sequence of this information by the AnimationManager::Animation
 * type.
 *
 * Also, an AnimationManager object contains a set of all the possible animations the sprite may need.
 * This set is represented by the AnimationSet class. Thus, a sprite needs but one AnimationManager object
 * to access any of the animations it requires, as long as these are all properly registered
 * in the AnimationSet object given to the AnimationManager object.
 */
class MediaPlayer {
  public:
    MediaPlayer();
    virtual ~MediaPlayer();

    virtual void Update(double dt) = 0;

    void AddTickFunction(std::function<void (void)> tick);

    void ChangeMediaManager(MediaManager*);

  protected:
    void notifyAllObservers();

  private:
    std::vector< std::function<void (void)> > ticks_;
    MediaManager* manager_;

    friend class MediaManager;
};

} /* namespace action */
} /* namespace ugdk */

#endif /* UGDK_ACTION_MEDIAPLAYER_H_ */


