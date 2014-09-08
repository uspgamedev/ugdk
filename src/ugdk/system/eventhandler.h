#ifndef UGDK_SYSTEM_EVENTHANDLER_H_
#define UGDK_SYSTEM_EVENTHANDLER_H_

#include <cassert>
#include <typeinfo>
#include <typeindex>
#include <functional>
#include <vector>
#include <memory>
#include <unordered_map>
#include <algorithm>
#include <ugdk/system/taskplayer.h>

namespace ugdk {
namespace system {

class IListener { public: virtual ~IListener() {} };

template <class Event>
class Listener : public IListener {
  public:
    template<typename Handler>
    Listener(Handler handler) : handler_(handler) {}

    void Handle(const Event& ev) {
        handler_(ev);
    }

  private:
    std::function<void (const Event&)> handler_;
};


class EventHandler {
  public:
    EventHandler() {}
    ~EventHandler() {}

    // Add listeners

    template<class Event, class Handler>
    IListener* AddListener(Handler handler) {
        return AddRawListener(typeid(Event), new Listener<Event>(handler));
    }

    template<class Event>
    IListener* AddListener(void (*handler)(const Event&)) {
        return AddRawListener(typeid(Event), new Listener<Event>(handler));
    }

    IListener* AddRawListener(const std::type_index& type, IListener* listener) {
        event_handlers_[type].emplace_back(listener);
        return listener;
    }

    // Remove listeners

    void RemoveListener(const std::type_index& type, IListener* listener) {
        auto handlers = event_handlers_.find(type);
        if(handlers == event_handlers_.end()) return;

        handlers->second.erase(
            std::remove_if(handlers->second.begin(), 
                           handlers->second.end(),
                           [listener](const std::shared_ptr<IListener>& val) {
                               return val.get() == listener;
                           }),
            handlers->second.end());
    }

    template<class Event>
    void RemoveListener(IListener* listener) {
        RemoveListener(typeid(Event), listener);
    }

    // Raising events
    template<class Event>
    void RaiseEvent(const Event& ev) const {
        auto handlers = event_handlers_.find(typeid(Event));
        if(handlers == event_handlers_.end()) return;

        for(const auto& listener : handlers->second) {
            Listener<Event>* specific_listener = dynamic_cast<Listener<Event>*>(listener.get());
            assert(specific_listener);
            specific_listener->Handle(ev);
        }
    }

  private:
    typedef std::vector< std::shared_ptr<IListener> > ListenerVector;

    std::unordered_map<std::type_index, ListenerVector> event_handlers_;
};

} // namespace system
} // namespace ugdk

#endif // UGDK_SYSTEM_EVENTHANDLER_H_
