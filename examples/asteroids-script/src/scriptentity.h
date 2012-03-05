#ifndef ASTEROIDS_SCRIPTENTITY_H
#define ASTEROIDS_SCRIPTENTITY_H

#include <string>
#include <ugdk/script/virtualobj.h>
#include <ugdk/action/entity.h>
#include <ugdk/util.h>

namespace ugdk {
	class Vector2D;
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

	ugdk::graphic::Node* node() const;

	bool has_new_objects();

	ugdk::script::VirtualObj new_objects();

	std::string type() const { return type_; }

	ugdk::Vector2D pos() const;

	double radius() const;

	ugdk::ikdtree::Box<2> GetBoundingBox();

	bool IsColliding(ScriptEntity* obj) const;

	void HandleCollision(ScriptEntity* target);

private:
    ugdk::script::VirtualObj proxy_;
	std::string type_;
};

}

#endif