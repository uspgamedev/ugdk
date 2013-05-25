#include <ugdk/action/mediaplayer.h>

#include <ugdk/action/observer.h>
#include <ugdk/action/mediamanager.h>

namespace ugdk {
namespace action {
    
MediaPlayer::MediaPlayer() : manager_(NULL) {}

MediaPlayer::~MediaPlayer() {
    if(manager_)
        manager_->RemovePlayer(this);
}

void MediaPlayer::AddObserver(Observer* observer) {
    observers_.push_back(observer);
}

void MediaPlayer::AddTickFunction(std::function<void (void)> tick) {
    ticks_.push_back(tick);
}
    
void MediaPlayer::ChangeMediaManager(MediaManager* manager) {
    manager->AddPlayer(this); // This function changes mangaer_
}

void MediaPlayer::notifyAllObservers() {
    for (int i = 0; i < (int)observers_.size(); ++i) {
        observers_[i]->Tick();
    }
    for(std::vector< std::function<void (void)> >::iterator it = ticks_.begin();
        it != ticks_.end(); ++it)
        (*it)();
}

} /* namespace action*/
} /* namespace ugdk */




