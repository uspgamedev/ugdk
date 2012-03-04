#ifndef ASTEROIDS_SCRIPTENTITYSTACK_H
#define ASTEROIDS_SCRIPTENTITYSTACK_H

#include <ugdk/script/virtualobj.h>

namespace asteroids {
	class ScriptEntity;
}

namespace asteroids {

class ScriptEntityStack {
public:
	ScriptEntityStack(const ugdk::script::VirtualObj& proxy);
	virtual ~ScriptEntityStack() {}
	
	int size();

	ScriptEntity* pop();
private:
    ugdk::script::VirtualObj proxy_;
};

}

#endif