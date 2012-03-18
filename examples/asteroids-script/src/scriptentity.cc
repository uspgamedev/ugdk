#include "scriptentity.h"
#include <ugdk/script/virtualobj.h>
#include <ugdk/action/entity.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/modifier.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/util/intervalkdtree.h>


namespace asteroids {

using ugdk::Vector2D;
using ugdk::script::VirtualObj;

ScriptEntity::ScriptEntity(const VirtualObj& proxy) : proxy_(proxy) {
	type_ = proxy_["type"].value<std::string>();
}

void ScriptEntity::Update(double dt) {
		if (is_destroyed()) { return; }
        VirtualObj vdt = VirtualObj(proxy_.wrapper());
        vdt.set_value(dt);
        std::list<VirtualObj> args;
        args.push_back(vdt);
        //proxy_["Update"](args);
        (proxy_ | "Update")(args);
}

bool ScriptEntity::is_destroyed() {
	return proxy_["is_destroyed"].value<bool>();
}

ugdk::graphic::Node* ScriptEntity::node() const {
	return proxy_["node"].value<ugdk::graphic::Node*>();
}

ugdk::graphic::Node* ScriptEntity::life_hud() const {
	VirtualObj life_hud = proxy_["life_hud"];
	if (life_hud)
		return life_hud["node"].value<ugdk::graphic::Node*>();
	return NULL;
}

ugdk::graphic::Node* ScriptEntity::energy_hud() const {
	VirtualObj energy_hud = proxy_["energy_hud"];
	if (energy_hud)
		return energy_hud["node"].value<ugdk::graphic::Node*>();
	return NULL;
}

bool ScriptEntity::has_new_objects() {
	std::list<VirtualObj> args;
	//return proxy_["new_objects"]["__len__"](args).value<int>() > 0;
	return (proxy_["new_objects"] | "__len__")(args).value<int>() > 0;
}

VirtualObj ScriptEntity::new_objects() {
	return proxy_["new_objects"];
}

Vector2D ScriptEntity::pos() const {
	return Vector2D(this->node()->modifier()->offset());
}

double ScriptEntity::radius() const {
	return proxy_["radius"].value<double>();
}

ugdk::ikdtree::Box<2> ScriptEntity::GetBoundingBox() {
	Vector2D thispos = this->pos();
	Vector2D thisposmin(thispos.x - this->radius(),
                        thispos.y - this->radius());
    Vector2D thisposmax(thispos.x + this->radius(),
                        thispos.y + this->radius());
    return ugdk::ikdtree::Box<2>(thisposmin.val, thisposmax.val);
}

bool ScriptEntity::IsColliding(ScriptEntity* obj) const {
	Vector2D distance = obj->pos() - pos();
	return distance.length() <= obj->radius() + radius();
}

void ScriptEntity::HandleCollision(ScriptEntity* target) {
	std::list<VirtualObj> args;
	args.push_back( VirtualObj(target->proxy_) );
	//proxy_["HandleCollision"](args);
    (proxy_ | "HandleCollision")(args);
}


}
