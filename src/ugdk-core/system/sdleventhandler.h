#ifndef UGDK_SYSTEM_SDLEVENTHANDLER_H_
#define UGDK_SYSTEM_SDLEVENTHANDLER_H_

#include "SDL_events.h"
#include <unordered_set>

namespace ugdk {
namespace system {

class SDLEventHandler {
  public:
    virtual std::unordered_set<Uint32> TypesHandled() const = 0;
    virtual void Handle(const ::SDL_Event&) const = 0;
};

}  // namespace system
}  // namespace ugdk

#endif // UGDK_SYSTEM_SDLEVENTHANDLER_H_
