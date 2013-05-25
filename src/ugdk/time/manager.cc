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

uint32 Manager::TimeElapsed() {
    return current_time_ - initial_time_;
}

uint32 Manager::TimeDifference() {
    return current_time_ - last_update_;
}

uint32 Manager::TimeSince(uint32 t0) {
    return current_time_ - t0 - initial_time_;
}

}  // namespace time
}  // namespace ugdk
