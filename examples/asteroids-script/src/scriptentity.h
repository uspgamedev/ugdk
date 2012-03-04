#ifndef ASTEROIDS_SCRIPTENTITY_H
#define ASTEROIDS_SCRIPTENTITY_H

#include <string>
#include <ugdk/script/virtualobj.h>
#include <ugdk/action/entity.h>

namespace ugdk {
namespace graphic {
	class Node;
}
}

namespace asteroids {

class ScriptEntity : public ugdk::Entity {
public:
	ScriptEntity(const ugdk::script::VirtualObj& proxy);
    virtual ~ScriptEntity() {}

    void Update(double dt);

	bool is_destroyed();

	ugdk::graphic::Node* get_node();

	bool has_new_objects();

	ugdk::script::VirtualObj get_new_objects();

	std::string name() const { return name_; }

private:
    ugdk::script::VirtualObj proxy_;
	std::string name_;
};

}

#endif