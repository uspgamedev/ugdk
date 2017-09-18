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

class IListener {
  public:
    virtual ~IListener();
  protected:
    IListener(const std::type_index& the_type) : handler_(nullptr), type_(the_type) {}
  private:
    friend class EventHandler;
    EventHandler *handler_;
    std::type_index type_;
};

template <class Event>
class Listener : public IListener {
  public:
    Listener() : IListener(typeid(Event)) {}

    virtual void Handle(const Event& ev) = 0;
};

template <class Event>
class FunctionListener : public Listener<Event> {
  public:
    template<typename Callback>
    FunctionListener(Callback the_callback) : Listener<Event>(), callback_(the_callback) {}

    void Handle(const Event& ev) override {
        callback_(ev);
    }

  private:
    std::function<void (const Event&)> callback_;
};

class EventHandler {
  public:
    EventHandler() : dispatch_as_global_(false) {}
    ~EventHandler() {
        for (auto& pair : event_listeners_)
            for (auto* listener : pair.second)
                listener->handler_ = nullptr;
    }

    // Add listeners

    template<class Event>
    void AddListener(Listener<Event> & listener) {
        event_listeners_[typeid(Event)].push_front(&listener);
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

        auto handlers = event_listeners_.find(typeid(Event));
        if (handlers != event_listeners_.end())
            for (const auto& listener : handlers->second) {
                Listener<Event>* specific_listener = dynamic_cast<Listener<Event>*>(listener);
                assert(specific_listener);
                specific_listener->Handle(ev);
            }
    }

  private:
    typedef std::forward_list< IListener* > ListenerVector;

    friend class IListener;

    std::unordered_map<std::type_index, ListenerVector> event_listeners_;
    bool dispatch_as_global_;

    template<class Event>
    void RemoveListener(IListener* listener) {
        RemoveListener(typeid(Event), listener);
    }

    void RemoveListener(const std::type_index& type, IListener* listener) {
        auto handlers = event_listeners_.find(type);
        if(handlers == event_listeners_.end()) return;

        handlers->second.remove_if([listener](const IListener* val) {
            return val == listener;
        });
    }
};

inline
IListener::~IListener() {
    if (handler_)
        handler_->RemoveListener(type_, this);
}

} // namespace system
} // namespace ugdk

#endif // UGDK_SYSTEM_EVENTHANDLER_H_
