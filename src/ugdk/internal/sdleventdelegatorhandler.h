#ifndef UGDK_INTERNAL_SDLEVENTDELEGATORHANDLER_H_
#define UGDK_INTERNAL_SDLEVENTDELEGATORHANDLER_H_

#include <ugdk/internal/sdleventhandler.h>
#include <vector>

namespace ugdk {
namespace internal {

class SDLEventDelegatorHandler : public SDLEventHandler {
  public:
    void AddHandler(const internal::SDLEventHandler* handler) {
        handlers_.push_back(handler);
    }

    bool CanHandle(const ::SDL_Event& sdlevent) const {
        for(auto handler : handlers_)
            if(handler->CanHandle(sdlevent))
                return true;
        return false;
    }
    
    void Handle(const ::SDL_Event& sdlevent) const {
        for(auto handler : handlers_)
            if(handler->CanHandle(sdlevent)) {
                handler->Handle(sdlevent);
                return;
            }
    }
  private:
    std::vector<const internal::SDLEventHandler*> handlers_;
};


}  // namespace internal
}  // namespace ugdk

#endif // UGDK_INTERNAL_SDLEVENTDELEGATORHANDLER_H_
