#include <ugdk/time/timemanager.h>

#include "SDL_timer.h"

namespace ugdk {
namespace time {

TimeManager::TimeManager() {
    last_update_ = current_time_ = initial_time_ = SDL_GetTicks();
}

void TimeManager::Update() {
    last_update_ = current_time_;
    current_time_ = SDL_GetTicks();
}

uint32 TimeManager::TimeElapsed() {
    return current_time_ - initial_time_;
}

uint32 TimeManager::TimeDifference() {
    return current_time_ - last_update_;
}

uint32 TimeManager::TimeSince(uint32 t0) {
    return current_time_ - t0 - initial_time_;
}

}  // namespace time
}  // namespace ugdk
