#ifndef UGDK_ACTION_MEDIAMANAGER_H_
#define UGDK_ACTION_MEDIAMANAGER_H_

#include <set>

#include <ugdk/action/mediaplayer.h>

namespace ugdk {
namespace action {

class MediaManager {
  public:
    MediaManager() {}
    ~MediaManager() {
        for(std::set<MediaPlayer*>::iterator it = players_.begin(); it != players_.end(); ++it)
            (*it)->manager_ = nullptr;
    }

    void Update(double dt) {
        for(std::set<MediaPlayer*>::iterator it = players_.begin(); it != players_.end(); ++it)
            (*it)->Update(dt);
    }

    void AddPlayer(MediaPlayer* player) {
        players_.insert(player);
        player->manager_ = this;
    }

    void RemovePlayer(MediaPlayer* player) {
        players_.erase(player);
        player->manager_ = nullptr;
    }

private:
    std::set<MediaPlayer*> players_;

};

} /* namespace action */
} /* namespace ugdk */

#endif /* UGDK_ACTION_MEDIAMANAGER_H_ */


