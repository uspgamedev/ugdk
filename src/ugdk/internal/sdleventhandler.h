#ifndef UGDK_INTERNAL_SDLEVENTHANDLER_H_
#define UGDK_INTERNAL_SDLEVENTHANDLER_H_

#include "SDL_events.h"
#include <unordered_set>

namespace ugdk {
namespace internal {

class SDLEventHandler {
  public:
    virtual std::unordered_set<Uint32> TypesHandled() const = 0;
    virtual void Handle(const ::SDL_Event&) const = 0;
};

}  // namespace internal
}  // namespace ugdk

#endif // UGDK_INTERNAL_SDLEVENTHANDLER_H_
