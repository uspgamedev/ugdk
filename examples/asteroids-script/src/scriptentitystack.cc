#include "scriptentitystack.h"
#include <ugdk/script/virtualobj.h>
#include "scriptentity.h"


namespace asteroids {

using ugdk::script::VirtualObj;

ScriptEntityStack::ScriptEntityStack(const VirtualObj& proxy) :
	proxy_(proxy) {
    objects_ = proxy_.value< std::list<VirtualObj> >();
}

int ScriptEntityStack::size() {
//	std::list<VirtualObj> args;
//    args.push_back(proxy_);
    //return (proxy_|"__len__")(args).value<int>();
//    return proxy_["__len__"](args).value<int>();
    return objects_.size();
}

ScriptEntity* ScriptEntityStack::pop() {
//	std::list<VirtualObj> args;
//    args.push_back(proxy_);
	//return new ScriptEntity( (proxy_|"pop")(args) );
//    return new ScriptEntity( proxy_["pop"](args) );
    VirtualObj obj = objects_.back();
    objects_.pop_back();
    return new ScriptEntity( obj );
}
}
