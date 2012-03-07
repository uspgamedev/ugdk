#include "scriptscene.h"
#include "scriptentity.h"
#include "scriptentitystack.h"
#include <ugdk/base/engine.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/graphic/node.h>
#include <ugdk/action/scene.h>
#include <ugdk/util/intervalkdtree.h>
#include <ugdk/input/inputmanager.h>
#include <ugdk/input/keys.h>
#include <ugdk/script/virtualobj.h>
#include <ugdk/script/scriptmanager.h>


namespace asteroids {

using ugdk::script::VirtualObj;

ScriptScene::ScriptScene() : ugdk::Scene() {
	map_generator_ = SCRIPT_MANAGER()->LoadModule("MapGenerator");

	double max = map_generator_["MAX_ENTITY_SIZE"].value<double>();

	double min_coordinates[2] = {-max, -max};
    double max_coordinates[2];
	max_coordinates[0] = ugdk::Engine::reference()->video_manager()->video_size().x + max;
    max_coordinates[1] = ugdk::Engine::reference()->video_manager()->video_size().y + max;

    ugdk::ikdtree::Box<2> tree_bounding_box(min_coordinates, max_coordinates);
    objects_tree_ = new ugdk::ikdtree::IntervalKDTree<ScriptEntity*, 2>(tree_bounding_box, 5);
}

void ScriptScene::GenerateMap() {
	std::vector<VirtualObj> args;
	ScriptEntityStack objects = ScriptEntityStack( map_generator_["Generate"](args) );

	this->AddNewObjects(objects);
}

ScriptEntityVector ScriptScene::FindCollidingObjects(ScriptEntity* target) const {
    std::vector<ScriptEntity*> *filtered_results 
        = objects_tree_->getIntersectingItems(target->GetBoundingBox());
	ScriptEntityVector result;

	std::vector<ScriptEntity *>::const_iterator it;
    for(it = filtered_results->begin(); it != filtered_results->end(); ++it) {
        if((*it) == target) continue;
		if( (*it)->IsColliding(target) && !target->is_destroyed())
            result.push_front(*it);
    }

    delete filtered_results;
    return result;
}

void ScriptScene::Update(double delta_t) {
	ScriptEntityVector to_delete;
	ScriptEntityVector::iterator it = script_entities_.begin();
	for (it = script_entities_.begin(); it != script_entities_.end(); ++it) {
		ScriptEntity* ent = *it;
		
		if (ent->has_new_objects()) {
			ScriptEntityStack objects = ScriptEntityStack( ent->new_objects() );
			this->AddNewObjects(objects);
		}
		
		if (ent->is_destroyed()) { 
			//printf("ScriptEntity %s is destroyed...\n", ent->type().c_str());
			this->RemoveEntity(ent);
			to_delete.push_back(ent);
			continue;
		}
		else { 
			objects_tree_->Update(ent->GetBoundingBox(), ent);
		}
	}

	it = to_delete.begin();
	for (it = to_delete.begin(); it != to_delete.end(); ++it) {
		ScriptEntity* ent = *it;
		script_entities_.remove(ent);
		objects_tree_->Remove(ent);
		delete ent->node();
		if (ent->life_hud())
    		delete ent->life_hud();
		if (ent->energy_hud())
			delete ent->energy_hud();
		delete ent;
	}

	it = script_entities_.begin();
	for (it = script_entities_.begin(); it != script_entities_.end(); ++it) {
		ScriptEntity* ent = *it;

		ScriptEntityVector collisions = FindCollidingObjects(ent);
		ScriptEntityVector::iterator colit = collisions.begin();
		for (colit = collisions.begin(); colit != collisions.end(); ++colit) {
			ScriptEntity* target = *colit;

			ent->HandleCollision(target);
		}
	}
	
	CheckCommands();

	Scene::Update(delta_t);	
}

void ScriptScene::CheckCommands() {
    if (INPUT_MANAGER()->KeyPressed(ugdk::input::K_ESCAPE)) {
        this->Finish();
    }
    else if (INPUT_MANAGER()->KeyPressed(ugdk::input::K_HOME)) {
        this->Finish();
        ScriptScene* scene = new ScriptScene();
        scene->GenerateMap();
        ugdk::Engine::reference()->PushScene(scene);
    }
}

void ScriptScene::AddNewObjects(ScriptEntityStack& objects) {
	ScriptEntity* ent;
	while ( objects.size() > 0 ) {
		ent = objects.pop();
		this->AddEntity(ent);
		script_entities_.push_back(ent);
		this->content_node()->AddChild(ent->node());
	    if (ent->life_hud())
		    this->interface_node()->AddChild(ent->life_hud());
		if (ent->energy_hud())
			this->interface_node()->AddChild(ent->energy_hud());
		objects_tree_->Insert(ent->GetBoundingBox(), ent);
	}
}

}
