#include "scriptentity.h"
#include <ugdk/script/virtualobj.h>
#include <ugdk/action/entity.h>
#include <ugdk/graphic/node.h>


namespace asteroids {

using ugdk::script::VirtualObj;

ScriptEntity::ScriptEntity(const VirtualObj& proxy) : proxy_(proxy) {
	name_ = proxy_["name"].value<std::string>();
}

void ScriptEntity::Update(double dt) {
		if (is_destroyed()) { return; }
        VirtualObj vdt = VirtualObj(proxy_.wrapper());
        vdt.set_value(dt);
        std::vector<VirtualObj> args;
        args.push_back(vdt);
        proxy_["Update"](args);
}

bool ScriptEntity::is_destroyed() {
	return proxy_["is_destroyed"].value<bool>();
}

ugdk::graphic::Node* ScriptEntity::get_node() {
	return proxy_["node"].value<ugdk::graphic::Node*>();
}

bool ScriptEntity::has_new_objects() {
	std::vector<VirtualObj> args;
	return proxy_["new_objects"]["__len__"](args).value<int>() > 0;
}

VirtualObj ScriptEntity::get_new_objects() {
	return proxy_["new_objects"];
}

}