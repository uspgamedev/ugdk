
#ifndef UGDK_SCRIPT_LUA_LUADATA_H_
#define UGDK_SCRIPT_LUA_LUADATA_H_

#include <ugdk/script/virtualdata.h>
#include <ugdk/script/lua/header.h>
#include <ugdk/script/lua/datagear.h>
#include <ugdk/script/lua/luawrapper.h>

#define GENERIC_WRAP(tname,arg) do { \
        DataGear& dtgear(wrapper_->data_gear()); \
        dtgear->push##tname(arg); \
        dtgear.SetData(id_); \
    } while(0)

namespace ugdk {
namespace script {
namespace lua {

class LuaData : public VirtualData {

  public:

    LuaData(LuaWrapper* wrapper, DataID id) :
        wrapper_(wrapper),
        id_(id) {}

    ~LuaData() {
        if (wrapper_)
            wrapper_->data_gear()//.DestroyID_old(id_);
                .SafeCall(DataGear::DestroyID)
                .Arg(id_)
                .NoResult();
    }

    void* Unwrap(const VirtualType& type) const;
    const char* UnwrapString() const {
        return UnwrapPrimitive<const char*>(NULL);
    }
    bool UnwrapBoolean() const {
        return UnwrapPrimitive<bool>(false);
    }
    int UnwrapInteger() const {
        return UnwrapPrimitive<int>(0);
    }
    double UnwrapNumber() const {
        return UnwrapPrimitive<double>(0.0);
    }

    void Wrap(void* data, const VirtualType& type);
    void WrapString(const char* str) { WrapPrimitive(str); }
    void WrapBoolean(bool boolean) { GENERIC_WRAP(boolean,boolean); }
    void WrapInteger(int number) { GENERIC_WRAP(integer,number); }
    void WrapNumber(double number) { GENERIC_WRAP(number,number); }

    LangWrapper* wrapper () const { return wrapper_; }

    Ptr Execute(const std::vector<Ptr>& args);

    Ptr GetAttribute(Ptr key);

    Ptr SetAttribute(Ptr key, Ptr value);

    void AddToBuffer();
    void RemoveFromBuffer();

  private:

    template <class T>
    T UnwrapPrimitive(const T default_value) const;

    template <class T>
    void WrapPrimitive(T value);

    LuaWrapper* wrapper_;
    DataID      id_;

};

template <class T>
T LuaData::UnwrapPrimitive(const T default_value) const {
    return wrapper_->data_gear()
        .SafeCall(DataGear::UnwrapPrimitive<T>)
        .Arg(id_)
        .GetResult(default_value);
}

template <class T>
void LuaData::WrapPrimitive(T value) {
    wrapper_->data_gear()
        .SafeCall(DataGear::WrapPrimitive<T>)
        .Arg(id_)
        .Arg(value)
        .NoResult();
}

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */

#endif /* UGDK_SCRIPT_LUA_LUADATA_H_ */
