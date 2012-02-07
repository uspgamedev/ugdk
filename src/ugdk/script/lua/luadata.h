
#ifndef UGDK_SCRIPT_LUA_LUADATA_H_
#define UGDK_SCRIPT_LUA_LUADATA_H_

#include <ugdk/script/virtualdata.h>
#include <ugdk/script/lua/header.h>
#include <ugdk/script/lua/datagear.h>
#include <ugdk/script/lua/luawrapper.h>

#define GENERIC_WRAP(tname,arg) do { \
        DataGear dtgear(wrapper_->MakeDataGear()); \
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
            wrapper_->MakeDataGear().DestroyID(id_);
    }

    void* Unwrap(const VirtualType& type) const;
    const char* UnwrapString() const;
    bool UnwrapBoolean() const;
    int UnwrapInteger() const;
    double UnwrapNumber() const;

    void Wrap(void* data, const VirtualType& type);
    void WrapString(const char* str) { GENERIC_WRAP(string,str); }
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

    LuaWrapper* wrapper_;
    DataID      id_;

};

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */

#endif /* UGDK_SCRIPT_LUA_LUADATA_H_ */
