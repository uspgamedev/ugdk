#ifndef MODULE_PROXY_TASK_H_
#define MODULE_PROXY_ENTITY_H_

#include <list>
#include <ugdk/script/baseproxy.h>

namespace ugdk {
namespace action {

class TaskProxy;
class TaskProxy : public ugdk::script::BaseProxy<TaskProxy> {
  public:
    TaskProxy(const ugdk::script::VirtualObj& proxy) : ugdk::script::BaseProxy<TaskProxy>(proxy) {}

    bool operator()(double dt) {
        std::list<ugdk::script::VirtualObj> args;
        args.emplace_front(proxy_.wrapper());
        args.front().set_value(dt);
        return ( proxy_ | "Update" )(args).value<bool>();
    }
};

}
}
#endif
