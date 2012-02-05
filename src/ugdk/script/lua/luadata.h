
#ifndef UGDK_SCRIPT_LUA_LUADATA_H_
#define UGDK_SCRIPT_LUA_LUADATA_H_

#include <ugdk/script/virtualdata.h>
#include <ugdk/script/lua/header.h>
#include <ugdk/script/lua/datagear.h>
#include <ugdk/script/lua/luawrapper.h>

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
            wrapper_->MakeDataGear().DestroyID(id_);
    }

    void* Unwrap(const VirtualType& type) const;

    void Wrap(void* data, const VirtualType& type);
    void Wrap(const char* str) { GenericWrap(str); }
    void Wrap(bool boolean) { GenericWrap(boolean); }
    void Wrap(int number) { GenericWrap(number); }
    void Wrap(double number) { GenericWrap(number); }

    LangWrapper* wrapper () const { return wrapper_; }

    Ptr Execute(const std::vector<Ptr>& args);

    Ptr GetAttribute(Ptr key);

    Ptr SetAttribute(Ptr key, Ptr value);

    void AddToBuffer();
    void RemoveFromBuffer();

  private:

    LuaWrapper* wrapper_;
    DataID      id_;

    template <class T>
    void GenericWrap(T data) {
        DataGear dtgear(wrapper_->MakeDataGear());
        dtgear->push<T>(data); // TODO TEST THIS!
        dtgear.SetData(id_);
    }

};

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */

#endif /* UGDK_SCRIPT_LUA_LUADATA_H_ */
