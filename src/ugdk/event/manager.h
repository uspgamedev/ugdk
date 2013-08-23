#ifndef UGDK_EVENT_MANAGER_H_
#define UGDK_EVENT_MANAGER_H_

#include <typeinfo>
#include <typeindex>
#include <functional>
#include <vector>
#include <memory>
#include <unordered_map>
#include <ugdk/action/scene.h>

namespace ugdk {
namespace event
{

class ITaskFactory {
  public:
    virtual ~ITaskFactory() {}
};

template <class Event>
class TaskFactory : public ITaskFactory {
  public:
    TaskFactory(std::function<ugdk::action::Task (const Event& ev)> factory)
        : factory_(factory) {}

    ugdk::action::Task Construct(const Event& ev) {
        return factory_(ev);
    }
  private:
    std::function<ugdk::action::Task (const Event& ev)> factory_;
};

class EventHandler {
  public:
    EventHandler(ITaskFactory* _factory, ugdk::action::Scene* _scene, double _priority)
        : factory_(_factory), scene_(_scene), priority_(_priority) {}

    ITaskFactory* factory() const { return factory_.get(); }
    action::Scene* scene() const { return scene_; }
    double priority() const { return priority_; }

  private:
    std::shared_ptr<ITaskFactory> factory_;
    ugdk::action::Scene* scene_;
    double priority_;
};

class Manager {
  public:
    Manager() {}
    ~Manager() {}

    template<class Event>
    void AddListener(TaskFactory<Event>* task_factory, ugdk::action::Scene* scene, double priority = 0.5) {
        event_handlers_[typeid(Event)].emplace_back(task_factory, scene, priority);
    }

    template<class Event>
    void RaiseEvent(const Event& ev) {
        for(const EventHandler& handler : event_handlers_[typeid(Event)]) {
            auto specific_factory = dynamic_cast<TaskFactory<Event>*>(handler.factory());
            assert(specific_factory);
            handler.scene()->AddTask(specific_factory->Construct(ev), handler.priority());
        }
    }

  private:
    std::unordered_map<std::type_index, std::vector<EventHandler> > event_handlers_;
};

} // namespace event
} // namespace ugdk

#endif // UGDK_EVENT_MANAGER_H_
