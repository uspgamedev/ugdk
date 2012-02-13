
#include <ugdk/script/lua/luadata.h>

namespace ugdk {
namespace script {
namespace lua {

using std::vector;

void* LuaData::Unwrap(const VirtualType& type) const {
    return wrapper_->data_gear()
        .SafeCall(DataGear::UnwrapData)
        .Arg(id_)
        .Arg(type.FromLang(LANG(Lua)))
        .GetResult<UData>(NULL);
}

const char* LuaData::UnwrapString() const {
    return wrapper_->data_gear().UnwrapString_old(id_);
}

bool LuaData::UnwrapBoolean() const {
    return wrapper_->data_gear().UnwrapBoolean(id_);
}

int LuaData::UnwrapInteger() const {
    return 0;
}

double LuaData::UnwrapNumber() const {
    return 0.0;
}


void LuaData::Wrap(void* data, const VirtualType& type) {
    if (!wrapper_->data_gear()
            .SafeCall(DataGear::WrapData)
            .Arg(id_)
            .Arg(data)
            .Arg(type.FromLang(LANG(Lua)))
            .NoResult())
        return; // TODO deal with error.
}

VirtualData::Ptr LuaData::Execute(const vector<Ptr>& args) {
    DataGear& dtgear = wrapper_->data_gear();
    dtgear.GetData(id_);
    vector<Ptr>::const_iterator it = args.begin();
    //wrapper_->Share(&dtgear);
    for(; it != args.end(); ++it)
        (*it)->AddToBuffer();
    //wrapper_->Share(NULL);
    dtgear->call(args.size(), 1);
    LuaData* result = wrapper_->NewLuaData();
    dtgear.SetData(result->id_);
    return Ptr(result);
}

VirtualData::Ptr LuaData::GetAttribute(Ptr key) {
    DataGear& dtgear = wrapper_->data_gear();
    dtgear.GetData(id_);
    //wrapper_->Share(&dtgear);
    key->AddToBuffer();
    //wrapper_->Share(NULL);
    dtgear->gettable(-2);
    if (dtgear->isnil(-1)) {
        puts("Failed to retrieve attribute!");
        dtgear->pop(2);
        return Ptr();
    }
    LuaData* value = wrapper_->NewLuaData();
    dtgear.SetData(value->id_);
    dtgear->pop(1);
    return Ptr(value);
}

VirtualData::Ptr LuaData::SetAttribute(Ptr key, Ptr value) {
    DataGear& dtgear = wrapper_->data_gear();   // []
    dtgear.GetData(id_);                // [data]
    //wrapper_->Share(&dtgear);
    key->AddToBuffer();             // [data,key]
    value->AddToBuffer();           // [data,key,value]
    dtgear->settable(-3);                // [data]
    // now get the stored value to return it
    key->AddToBuffer();             // [data,key]
    //wrapper_->Share(NULL);
    dtgear->gettable(-2);                // [data,data.key]
    if (dtgear->isnil(-1)) {
        dtgear->pop(2);                  // []
        return Ptr();
    }
    LuaData* stored_value = wrapper_->NewLuaData();
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
