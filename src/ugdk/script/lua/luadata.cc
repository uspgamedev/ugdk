
#include <algorithm>
#include <functional>

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
    std::for_each(
        args.begin(),
        args.end(),
        std::tr1::mem_fn(&VirtualData::AddToBuffer)
    );
    return wrapper_->OperateBuffer(id_, DataGear::Execute);
}

VirtualData::Ptr LuaData::GetAttribute(Ptr key) {
    key->AddToBuffer();
    return wrapper_->OperateBuffer(id_, DataGear::GetField);
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
    //wrapper_->shared_gear()->GetData(id_);
    wrapper_->AddToBuffer(id_);
}

void LuaData::RemoveFromBuffer() {
    if (!wrapper_->shared_gear()->SetData(id_))
        puts("WARNING! Bad lua data badly generated!");
}

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */
