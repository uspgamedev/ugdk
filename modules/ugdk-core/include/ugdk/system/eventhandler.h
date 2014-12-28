#ifndef UGDK_SYSTEM_EVENTHANDLER_H_
#define UGDK_SYSTEM_EVENTHANDLER_H_

#include <ugdk/system/engine.h>

#include <typeinfo>
#include <typeindex>
#include <functional>
#include <forward_list>
#include <memory>
#include <unordered_map>
#include <algorithm>
#include <cassert>

namespace ugdk {
namespace system {

class IListener { public: virtual ~IListener() {} };

template <class Event>
class Listener : public virtual IListener {
  public:
    virtual void Handle(const Event& ev) = 0;
};

template <class Event>
class FunctionListener : public Listener<Event> {
  public:
    template<typename Handler>
    FunctionListener(Handler handler) : handler_(handler) {}

    void Handle(const Event& ev) override {
        handler_(ev);
    }

  private:
    std::function<void (const Event&)> handler_;
};

class EventHandler {
  public:
    EventHandler() : dispatch_as_global_(false) {}
    ~EventHandler() {}

    // Add listeners

    template<class Event, class Handler>
    IListener* AddListener(Handler handler) {
        return AddRawListener(typeid(Event), std::make_shared<FunctionListener<Event>>(handler));
    }

    template<class Event>
    IListener* AddListener(void (*handler)(const Event&)) {
        return AddRawListener(typeid(Event), std::make_shared<FunctionListener<Event>>(handler));
    }

    IListener* AddRawListener(const std::type_index& type, const std::shared_ptr<IListener>& listener) {
        event_handlers_[type].push_front(listener);
        return listener.get();
    }

    /** Adds an object to listen to all events dispatched to this EventHandler.
        The object must implement the Listener<T> interface for the events it wants to track.
        @param listener The object to dispatch events to. This class does not delete the
                        received pointer and should be notified when it becomes invalid via
                        RemoveObjectListener.
    */
    void AddObjectListener(IListener* listener) {
        object_listeners_.push_front(listener);
    }

    // Remove listeners

    void RemoveListener(const std::type_index& type, IListener* listener) {
        auto handlers = event_handlers_.find(type);
        if(handlers == event_handlers_.end()) return;

        handlers->second.remove_if([listener](const std::shared_ptr<IListener>& val) {
            return val.get() == listener;
        });
    }

    template<class Event>
    void RemoveListener(IListener* listener) {
        RemoveListener(typeid(Event), listener);
    }

    void RemoveObjectListener(IListener* listener) {
        object_listeners_.remove(listener);
    }
    
    /// If true, events raised on this handler will be raised as global events.
    void set_dispatch_as_global(bool flag) {
        dispatch_as_global_ = flag;
    }

    /// Dispatches a global event.
    /** Globals events are received by the global EventHandler and
    *   the CurrentScene's EventHandler.
    */
    template<class Event>
    static void RaiseGlobalEvent(const Event& ev) {
        static bool processing_global_event = false;
        if (processing_global_event) return;

        processing_global_event = true;
        GlobalEventHandler().RaiseEvent(ev);
        GetCurrentSceneEventHandler().RaiseEvent(ev);
        processing_global_event = false;
    }

    // Raising events
    template<class Event>
    void RaiseEvent(const Event& ev) const {
        if (dispatch_as_global_)
            RaiseGlobalEvent(ev);

        auto handlers = event_handlers_.find(typeid(Event));
        if (handlers != event_handlers_.end())
            for (const auto& listener : handlers->second) {
                Listener<Event>* specific_listener = dynamic_cast<Listener<Event>*>(listener.get());
                assert(specific_listener);
                specific_listener->Handle(ev);
            }

        for (IListener* listener : object_listeners_) {
            if (Listener<Event>* specific_listener = dynamic_cast<Listener<Event>*>(listener)) {
                specific_listener->Handle(ev);
            }
        }
    }

  private:
    typedef std::forward_list< std::shared_ptr<IListener> > ListenerVector;

    std::unordered_map<std::type_index, ListenerVector> event_handlers_;
    std::forward_list<IListener*> object_listeners_;
    bool dispatch_as_global_;
};

} // namespace system
} // namespace ugdk

#endif // UGDK_SYSTEM_EVENTHANDLER_H_
