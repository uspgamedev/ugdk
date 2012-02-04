
#include <ugdk/script/lua/luadata.h>

namespace ugdk {
namespace script {
namespace lua {

using std::vector;

void* LuaData::Unwrap(const VirtualType& type) const {
    Gear g(wrapper_->MakeGear());
    if(!g.GetData(id())) return NULL;
    return g.UnwrapData(type);
}

void LuaData::Wrap(void* data, const VirtualType& type) {
    Gear g(wrapper_->MakeGear());
    g.WrapData(data, type);
    if (!g.SetData(id()))
        return; // TODO deal with error.
}

VirtualData::Ptr LuaData::Execute(const vector<Ptr>& args) {
    Gear g(wrapper_->MakeGear());
    g.GetData(id());
    vector<Ptr>::const_iterator it = args.begin();
    wrapper_->Persist(&g);
    for(; it != args.end(); ++it)
        (*it)->AddToBuffer();
    wrapper_->Persist(NULL);
    g->call(args.size(), 1);
    LuaData* result = new LuaData(wrapper_);
    g.SetData(result->id());
    return VirtualData::Ptr(result);
}

/// Protected:

void LuaData::AddToBuffer() {
    wrapper_->persistent_gear()->GetData(id());
}

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */
