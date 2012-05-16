#ifndef MODULE_PROXY_TASK_H_
#define MODULE_PROXY_ENTITY_H_

#include <ugdk/action/task.h>
#include <ugdk/script/baseproxy.h>
#include <list>

namespace ugdk {
namespace action {

class TaskProxy;
class TaskProxy : public Task, public ugdk::script::BaseProxy<TaskProxy> {
  public:
    TaskProxy(const ugdk::script::VirtualObj& proxy) : ugdk::script::BaseProxy<TaskProxy>(proxy) {
		ugdk::script::VirtualObj vpriori = proxy["priority"];
		if (vpriori) {
			priority_ = vpriori.value<int>();
		}
	}

    virtual void operator()(double dt) {
        ugdk::script::VirtualObj vdt = ugdk::script::VirtualObj(proxy_.wrapper());
        vdt.set_value(dt);
        std::list<ugdk::script::VirtualObj> args;
        args.push_back(vdt);
        ugdk::script::VirtualObj ret = proxy_(args);
		finished_ = !ret.value<bool>();
    }
};

}
}
#endif
