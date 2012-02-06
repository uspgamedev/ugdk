
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
    wrapper_->Share(&g);
    for(; it != args.end(); ++it)
        (*it)->AddToBuffer();
    wrapper_->Share(NULL);
    g->call(args.size(), 1);
    LuaData* result = new LuaData(wrapper_);
    g.SetData(result->id());
    return Ptr(result);
}

VirtualData::Ptr LuaData::GetAttribute(Ptr key) {
    Gear g(wrapper_->MakeGear());
    g.GetData(id());
    wrapper_->Share(&g);
    key->AddToBuffer();
    wrapper_->Share(NULL);
    g->gettable(-2);
    if (g->isnil(-1)) {
        g->pop(2);
        return Ptr();
    }
    LuaData* value = new LuaData(wrapper_);
    g.SetData(value->id());
    g->pop(1);
    return Ptr(value);
}

VirtualData::Ptr LuaData::SetAttribute(Ptr key, Ptr value) {
    Gear g(wrapper_->MakeGear());   // []
    g.GetData(id());                // [data]
    wrapper_->Share(&g);
    key->AddToBuffer();             // [data,key]
    value->AddToBuffer();           // [data,key,value]
    g->settable(-3);                // [data]
    // now get the stored value to return it
    key->AddToBuffer();             // [data,key]
    wrapper_->Share(NULL);
    g->gettable(-2);                // [data,data.key]
    if (g->isnil(-1)) {
        g->pop(2);                  // []
        return Ptr();
    }
    LuaData* stored_value = new LuaData(wrapper_);
    g.SetData(stored_value->id());  // [data]
    g->pop(1);                      // []
    return Ptr(stored_value);
}

void LuaData::AddToBuffer() {
    wrapper_->shared_gear()->GetData(id());
}

void LuaData::RemoveFromBuffer() {
    if (!wrapper_->shared_gear()->SetData(id()))
        puts("WARNING! Bad lua data badly generated!");
}

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */
