#ifndef UGDK_SYSTEM_EVENTHANDLER_H_
#define UGDK_SYSTEM_EVENTHANDLER_H_

#include <cassert>
#include <typeinfo>
#include <typeindex>
#include <functional>
#include <vector>
#include <memory>
#include <unordered_map>
#include <ugdk/system/taskplayer.h>

namespace ugdk {
namespace system {

struct ITaskFactory { virtual ~ITaskFactory() {} };

template <class Event>
class TaskFactory : public ITaskFactory {
  public:
    template<typename EventTask>
    TaskFactory(EventTask task)
        : task_(task) {}

    template<typename Filter>
    void set_filter(Filter f) {
        filter_ = f;
    }

    bool Filter(const Event& ev) {
        return static_cast<bool>(filter_) ? filter_(ev) : true;
    }

    Task Construct(const Event& ev) {
        return [this, ev](double dt) {
            return this->task_(ev, dt);
        };
    }
  private:
    std::function<bool (const Event&, double)> task_;
    std::function<bool (const Event&)> filter_;
};


class EventHandler {
  private:
    class EventAdapter {
      public:
        EventAdapter(ITaskFactory* _factory, double _priority)
            : factory_(_factory), priority_(_priority) {}

        ITaskFactory* factory() const { return factory_.get(); }
        double priority() const { return priority_; }

      private:
        std::shared_ptr<ITaskFactory> factory_;
        double priority_;
    };

  public:
    EventHandler(TaskPlayer* task_player) : task_player_(task_player) {}
    ~EventHandler() {}

    template<class Event, class EventTask>
    void AddListener(EventTask event_task, double priority = 0.5) {
        AddListener(
            typeid(Event),
            new TaskFactory<Event>(event_task),
            priority);
    }

    void AddListener(const std::type_index& type, ITaskFactory* factory, double priority) {
        event_handlers_[type].emplace_back(factory, priority);
    }

    template<class Event>
    void RaiseEvent(const Event& ev) const {
        auto handlers = event_handlers_.find(typeid(Event));
        if(handlers != event_handlers_.end())
            for(const EventAdapter& handler : handlers->second) {
                auto specific_factory = dynamic_cast<TaskFactory<Event>*>(handler.factory());
                assert(specific_factory);
                if(specific_factory->Filter(ev))
                    task_player_->AddTask(specific_factory->Construct(ev));
            }
    }

  private:
    TaskPlayer* task_player_;
    std::unordered_map<std::type_index, std::vector<EventAdapter> > event_handlers_;
};

} // namespace system
} // namespace ugdk

#endif // UGDK_SYSTEM_EVENTHANDLER_H_
