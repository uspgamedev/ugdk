
#include <ugdk/script/lua/luadata.h>

namespace ugdk {
namespace script {
namespace lua {

using std::vector;

void* LuaData::Unwrap(const VirtualType& type) const {
    return wrapper_->MakeDataGear().UnwrapData(id_, type);
}

void LuaData::Wrap(void* data, const VirtualType& type) {
    if (!wrapper_->MakeDataGear().WrapData(id_, data, type))
        return; // TODO deal with error.
}

VirtualData::Ptr LuaData::Execute(const vector<Ptr>& args) {
    DataGear dtgear(wrapper_->MakeDataGear());
    dtgear.GetData(id_);
    vector<Ptr>::const_iterator it = args.begin();
    wrapper_->Share(&dtgear);
    for(; it != args.end(); ++it)
        (*it)->AddToBuffer();
    wrapper_->Share(NULL);
    dtgear->call(args.size(), 1);
    LuaData* result = new LuaData(wrapper_, dtgear.GenerateID());
    dtgear.SetData(result->id_);
    return Ptr(result);
}

VirtualData::Ptr LuaData::GetAttribute(Ptr key) {
    DataGear dtgear(wrapper_->MakeDataGear());
    dtgear.GetData(id_);
    wrapper_->Share(&dtgear);
    key->AddToBuffer();
    wrapper_->Share(NULL);
    dtgear->gettable(-2);
    if (dtgear->isnil(-1)) {
        puts("Failed to retrieve attribute!");
        dtgear->pop(2);
        return Ptr();
    }
    LuaData* value = new LuaData(wrapper_, dtgear.GenerateID());
    dtgear.SetData(value->id_);
    dtgear->pop(1);
    return Ptr(value);
}

VirtualData::Ptr LuaData::SetAttribute(Ptr key, Ptr value) {
    DataGear dtgear(wrapper_->MakeDataGear());   // []
    dtgear.GetData(id_);                // [data]
    wrapper_->Share(&dtgear);
    key->AddToBuffer();             // [data,key]
    value->AddToBuffer();           // [data,key,value]
    dtgear->settable(-3);                // [data]
    // now get the stored value to return it
    key->AddToBuffer();             // [data,key]
    wrapper_->Share(NULL);
    dtgear->gettable(-2);                // [data,data.key]
    if (dtgear->isnil(-1)) {
        dtgear->pop(2);                  // []
        return Ptr();
    }
    LuaData* stored_value = new LuaData(wrapper_, dtgear.GenerateID());
    dtgear.SetData(stored_value->id_);  // [data]
    dtgear->pop(1);                      // []
    return Ptr(stored_value);
}

void LuaData::AddToBuffer() {
    wrapper_->shared_gear()->GetData(id_);
}

void LuaData::RemoveFromBuffer() {
    if (!wrapper_->shared_gear()->SetData(id_))
        puts("WARNING! Bad lua data badly generated!");
}

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */
