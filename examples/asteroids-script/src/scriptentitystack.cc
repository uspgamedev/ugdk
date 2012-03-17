#include "scriptentitystack.h"
#include <ugdk/script/virtualobj.h>
#include "scriptentity.h"


namespace asteroids {

using ugdk::script::VirtualObj;

ScriptEntityStack::ScriptEntityStack(const VirtualObj& proxy) :
	proxy_(proxy) {}

int ScriptEntityStack::size() {
	std::list<VirtualObj> args;
    return (proxy_|"__len__")(args).value<int>();
}

ScriptEntity* ScriptEntityStack::pop() {
	std::list<VirtualObj> args;
	return new ScriptEntity( (proxy_|"pop")(args) );
}
}
