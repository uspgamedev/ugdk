#ifndef ASTEROIDS_SCRIPTSCENE_H
#define ASTEROIDS_SCRIPTSCENE_H

#include <list>
#include <ugdk/action/scene.h>
#include "scriptentity.h"
#include "scriptentitystack.h"
#include <ugdk/util.h>

namespace ugdk {
namespace script {
	class VirtualObj;
}
}

namespace asteroids {

typedef std::list<ScriptEntity*> ScriptEntityVector;

class ScriptScene : public ugdk::Scene {
public:
	ScriptScene();
	virtual ~ScriptScene();

	void GenerateMap();

	ScriptEntityVector FindCollidingObjects(ScriptEntity* target) const;

	void Update(double delta_t);

    void CheckCommands();

	void AddNewObjects(ScriptEntityStack& objects);

private:
	ugdk::script::VirtualObj map_generator_;
	ScriptEntityVector script_entities_;

	ugdk::ikdtree::IntervalKDTree<ScriptEntity*, 2>* objects_tree_;
};

}

#endif
