#include <ugdk/time/manager.h>

#include "SDL_timer.h"

namespace ugdk {
namespace time {

Manager::Manager() {
    last_update_ = current_time_ = initial_time_ = SDL_GetTicks();
}

void Manager::Update() {
    last_update_ = current_time_;
    current_time_ = SDL_GetTicks();
}

uint32 Manager::TimeElapsed() const {
    return current_time_ - initial_time_;
}

uint32 Manager::TimeElapsedInFrame() const{
    return SDL_GetTicks() - current_time_;
}

uint32 Manager::TimeDifference() const {
    return current_time_ - last_update_;
}

uint32 Manager::TimeSince(uint32 t0) const {
    return current_time_ - t0 - initial_time_;
}

}  // namespace time
}  // namespace ugdk
