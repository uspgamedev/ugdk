#ifndef ASTEROIDS_SCRIPTENTITYSTACK_H
#define ASTEROIDS_SCRIPTENTITYSTACK_H

#include <ugdk/script/virtualobj.h>
#include <list>

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
    std::list<ugdk::script::VirtualObj> objects_;
};

}

#endif
