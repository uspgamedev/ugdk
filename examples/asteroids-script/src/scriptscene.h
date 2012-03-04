#ifndef ASTEROIDS_SCRIPTSCENE_H
#define ASTEROIDS_SCRIPTSCENE_H

#include <list>
#include <ugdk/action/scene.h>
#include "scriptentity.h"

namespace ugdk {
namespace script {
	class VirtualObj;
}
}

namespace asteroids {

class ScriptScene : public ugdk::Scene {
public:
	ScriptScene();

	void GenerateMap();

	void Update(double delta_t);

private:
	ugdk::script::VirtualObj map_generator_;
	typedef std::list<ScriptEntity*> ScriptEntityVector;
	ScriptEntityVector script_entities_;
};

}

#endif