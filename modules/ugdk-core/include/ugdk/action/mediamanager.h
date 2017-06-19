#ifndef UGDK_ACTION_MEDIAMANAGER_H_
#define UGDK_ACTION_MEDIAMANAGER_H_

#include <set>
#include <memory>

#include <ugdk/action/mediaplayer.h>

namespace ugdk {
namespace action {

class MediaManager {
  public:
    MediaManager() {}
    ~MediaManager() {
        for(std::shared_ptr<MediaPlayer> it : players_)
            it->manager_ = nullptr;
    }

    void Update(double dt) {
        for(std::shared_ptr<MediaPlayer> it : players_)
            it->Update(dt);
    }

    void AddPlayer(std::shared_ptr<MediaPlayer> player) {
        players_.insert(player);
        player->manager_ = this;
    }

    void RemovePlayer(std::shared_ptr<MediaPlayer> player) {
        players_.erase(player);
        player->manager_ = nullptr;
    }

private:
    std::set< std::shared_ptr<MediaPlayer> > players_;

};

} /* namespace action */
} /* namespace ugdk */

#endif /* UGDK_ACTION_MEDIAMANAGER_H_ */


