#ifndef UGDK_ACTION_MEDIAPLAYER_H_
#define UGDK_ACTION_MEDIAPLAYER_H_

#include <vector>
#include <string>

#include <functional>

#include <ugdk/action.h>

#define DEFAULT_PERIOD 0.1

namespace ugdk {
namespace action {

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

    /// Add a observer object to the animation
    /** Given an observer object, the function include this in the animation manager
    *  @param *observer is a pointer to the observer object
    */
    void AddObserver(Observer* observer);

    void AddTickFunction(std::function<void (void)> tick);

  protected:
    void notifyAllObservers();

  private:
    std::vector<Observer *> observers_;
    std::vector< std::function<void (void)> > ticks_;
};

} /* namespace action */
} /* namespace ugdk */

#endif /* UGDK_ACTION_MEDIAPLAYER_H_ */


