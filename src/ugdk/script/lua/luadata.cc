
#include <algorithm>
#include <functional>

#include <ugdk/script/lua/luadata.h>
#include <ugdk/script/lua/datagear.h>

namespace ugdk {
namespace script {
namespace lua {

using std::vector;

LuaData::~LuaData() {
    if (wrapper_)
        wrapper_->data_gear()//.DestroyID_old(id_);
            .SafeCall(DataGear::DestroyID)
            .Arg(id_)
            .NoResult();
}

void* LuaData::Unwrap(const VirtualType& type) const {
    return wrapper_->data_gear()
        .SafeCall(DataGear::UnwrapData)
        .Arg(id_)
        .Arg(type.FromLang(LANG(Lua)))
        .GetResult<UData>(NULL);
}

template <class T>
T UnwrapPrimitive(LuaWrapper* wrapper, const DataID id, const T default_value) {
    return wrapper->data_gear()
        .SafeCall(DataGear::UnwrapPrimitive<T>)
        .Arg(id)
        .GetResult(default_value);
}

const char* LuaData::UnwrapString() const {
    return UnwrapPrimitive<const char*>(wrapper_, id_, NULL);
}

bool LuaData::UnwrapBoolean() const {
    return UnwrapPrimitive<bool>(wrapper_, id_, false);
}

int LuaData::UnwrapInteger() const {
    return UnwrapPrimitive<int>(wrapper_, id_, 0);
}

double LuaData::UnwrapNumber() const {
    return UnwrapPrimitive<double>(wrapper_, id_, 0.0);
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

template <class T>
void WrapPrimitive(LuaWrapper* wrapper, DataID id, T value) {
    wrapper->data_gear()
        .SafeCall(DataGear::WrapPrimitive<T>)
        .Arg(id)
        .Arg(value)
        .NoResult();
}

void LuaData::WrapString(const char* str) {
    WrapPrimitive(wrapper_, id_, str);
}

void LuaData::WrapBoolean(bool boolean) {
    WrapPrimitive(wrapper_, id_, boolean);
}

void LuaData::WrapInteger(int number) {
    WrapPrimitive(wrapper_, id_, number);
}

void LuaData::WrapNumber(double number) {
    WrapPrimitive(wrapper_, id_, number);
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
    key->AddToBuffer();
    value->AddToBuffer();
    return wrapper_->OperateBuffer(id_, DataGear::SetField);
}

void LuaData::AddToBuffer() {
    wrapper_->AddToBuffer(id_);
}

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */
