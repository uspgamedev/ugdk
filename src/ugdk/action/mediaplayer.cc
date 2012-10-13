#include <ugdk/action/mediaplayer.h>

#include <ugdk/action/observer.h>

namespace ugdk {
namespace action {
    
MediaPlayer::MediaPlayer() {}

MediaPlayer::~MediaPlayer() {}

void MediaPlayer::AddObserver(Observer* observer) {
    observers_.push_back(observer);
}

void MediaPlayer::AddTickFunction(std::tr1::function<void (void)> tick) {
    ticks_.push_back(tick);
}

void MediaPlayer::notifyAllObservers() {
    for (int i = 0; i < (int)observers_.size(); ++i) {
        observers_[i]->Tick();
    }
    for(std::vector< std::tr1::function<void (void)> >::iterator it = ticks_.begin();
        it != ticks_.end(); ++it)
        (*it)();
}

} /* namespace action*/
} /* namespace ugdk */




