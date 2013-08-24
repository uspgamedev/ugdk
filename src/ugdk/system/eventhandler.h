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
    TaskFactory(std::function<Task (const Event& ev)> factory)
        : factory_(factory) {}

    Task Construct(const Event& ev) {
        return factory_(ev);
    }
  private:
    std::function<Task (const Event& ev)> factory_;
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

    template<class Event>
    void AddListener(std::function<Task (const Event& ev)> task_factory, double priority = 0.5) {
        event_handlers_[typeid(Event)].emplace_back(
            new TaskFactory<Event>(task_factory),
            priority);
    }

    template<class Event>
    void RaiseEvent(const Event& ev) {
        for(const EventAdapter& handler : event_handlers_[typeid(Event)]) {
            auto specific_factory = dynamic_cast<TaskFactory<Event>*>(handler.factory());
            assert(specific_factory);
            task_player_->AddTask(specific_factory->Construct(ev), handler.priority());
        }
    }

  private:
    TaskPlayer* task_player_;
    std::unordered_map<std::type_index, std::vector<EventAdapter> > event_handlers_;
};

} // namespace system
} // namespace ugdk

#endif // UGDK_SYSTEM_EVENTHANDLER_H_
