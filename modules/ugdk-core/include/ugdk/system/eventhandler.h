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
    ~Listener();

    virtual void Handle(const Event& ev) = 0;
  private:
    friend class EventHandler;
    EventHandler * handler_;
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

    template<class Event>
    void AddListener(const Listener<Event>& listener) {
        event_handlers_[typeid(Event)].push_front(&listener);
        listener.handler_ = this;
    }


    /** Adds an object to listen to all events dispatched to this EventHandler.
        The object must implement the Listener<T> interface for the events it wants to track.
        @param listener The object to dispatch events to. This class does not delete the
                        received pointer and should be notified when it becomes invalid via
                        RemoveObjectListener.
    */


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
                Listener<Event>* specific_listener = dynamic_cast<Listener<Event>*>(listener);
                assert(specific_listener);
                specific_listener->Handle(ev);
            }
    }

  private:
    typedef std::forward_list< IListener* > ListenerVector;
    
    template <class Event>
    friend class Listener;

    std::unordered_map<std::type_index, ListenerVector> event_handlers_;
    bool dispatch_as_global_;

    template<class Event>
    void RemoveListener(IListener* listener) {
        std::type_index type = typeid(Event);
        auto handlers = event_handlers_.find(type);
        if(handlers == event_handlers_.end()) return;

        handlers->second.remove_if([listener](const IListener* val) {
            return val == listener;
        });
    }
};

template<class Event>
Listener<Event>::~Listener() {
    if (handler_)
        handler_->RemoveListener< Event>(this);
}
} // namespace system
} // namespace ugdk

#endif // UGDK_SYSTEM_EVENTHANDLER_H_
