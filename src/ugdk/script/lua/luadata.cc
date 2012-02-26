
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
