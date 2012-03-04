#include "scriptscene.h"
#include "scriptentity.h"
#include "scriptentitystack.h"
#include <ugdk/graphic/node.h>
#include <ugdk/action/scene.h>
#include <ugdk/script/virtualobj.h>
#include <ugdk/script/scriptmanager.h>


namespace asteroids {

using ugdk::script::VirtualObj;

ScriptScene::ScriptScene() : ugdk::Scene() {
	map_generator_ = SCRIPT_MANAGER()->LoadModule("MapGenerator");
}

void ScriptScene::GenerateMap() {
	std::vector<VirtualObj> args;
	ScriptEntityStack objects = ScriptEntityStack( map_generator_["Generate"](args) );

	ScriptEntity* ent;
	while ( objects.size() > 0 ) {
		ent = objects.pop();
		this->AddEntity(ent);
		script_entities_.push_back(ent);
		this->content_node()->AddChild(ent->get_node());
	}
}

void ScriptScene::Update(double delta_t) {
	ScriptEntityVector to_delete;
	ScriptEntityVector::iterator it = script_entities_.begin();
	for (it = script_entities_.begin(); it != script_entities_.end(); ++it) {
		ScriptEntity* ent = *it;

		if (ent->is_destroyed()) { 
			printf("ScriptEntity %s is destroyed...\n", ent->name().c_str());
			this->RemoveEntity(ent);
			to_delete.push_back(ent);
			continue;
		}
		else {
			if (ent->has_new_objects()) {
				ScriptEntityStack objects = ScriptEntityStack( ent->get_new_objects() );
				ScriptEntity* ent;
				while ( objects.size() > 0 ) {
					ent = objects.pop();
					this->AddEntity(ent);
					script_entities_.push_back(ent);
					this->content_node()->AddChild(ent->get_node());
				}
			}
		}
	}

	it = to_delete.begin();
	for (it = to_delete.begin(); it != to_delete.end(); ++it) {
		ScriptEntity* ent = *it;
		script_entities_.remove(ent);
		delete ent->get_node();
		delete ent;
	}

	Scene::Update(delta_t);	
}

}