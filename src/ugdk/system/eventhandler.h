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
    template<typename EventTask>
    Listener(EventTask task, double priority)
        : priority_(priority), event_task_(task) {}

    template<typename Filter>
    void set_filter(Filter f) {
        filter_ = f;
    }

    double priority() const {
        return priority_;
    }

    bool Filter(const Event& ev) const {
        return static_cast<bool>(filter_) ? filter_(ev) : true;
    }

    void Handle(const Event& ev) {
        event_task_(ev);
    }

  private:
    double priority_;
    std::function<void (const Event&)> event_task_;
    std::function<bool (const Event&)> filter_;
};


class EventHandler {
  public:
    EventHandler(TaskPlayer* task_player) : task_player_(task_player) {}
    ~EventHandler() {}

    // Add listeners

    template<class Event, class EventTask>
    IListener* AddListener(EventTask event_task, double priority = 0.5) {
        return AddListener(
            typeid(Event),
            new Listener<Event>(event_task, priority));
    }

    IListener* AddListener(const std::type_index& type, IListener* listener) {
        event_handlers_[type].emplace_back(listener);
        return listener;
    }

    // Remove listeners

    void RemoveListener(const std::type_index& type, IListener* listener) {
        auto handlers = event_handlers_.find(type);
        if(handlers == event_handlers_.end()) return;

        handlers->second.erase(std::remove_if(handlers->second.begin(), handlers->second.end(),
            [listener](const std::shared_ptr<IListener>& val) { return val.get() == listener; }), handlers->second.end());
    }

    template<class Event>
    void RemoveListener(IListener* listener) {
        RemoveListener(typeid(Event), listener);
    }
    

    // Raising events

    // Schedule (create Task)

    template<class Event>
    void ScheduleEvent(const Event& ev) const {
        auto handlers = event_handlers_.find(typeid(Event));
        if(handlers == event_handlers_.end()) return;

        for(const auto& listener : handlers->second)
            ScheduleEvent(ev, dynamic_cast<Listener<Event>*>(listener.get()));
    }

  private:
    template<class Event>
    void ScheduleEvent(const Event& ev, Listener<Event>* specific_listener) const {
        assert(specific_listener);
        if(specific_listener->Filter(ev)) {
            task_player_->AddTask(Task([ev,specific_listener](double){
                specific_listener->Handle(ev);
                return false;
            }, specific_listener->priority()));
        }
    }


    TaskPlayer* task_player_;
    std::unordered_map<std::type_index, std::vector<std::shared_ptr<IListener>> > event_handlers_;
};

} // namespace system
} // namespace ugdk

#endif // UGDK_SYSTEM_EVENTHANDLER_H_
