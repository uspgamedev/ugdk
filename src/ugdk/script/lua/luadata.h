
#ifndef UGDK_SCRIPT_LUA_LUADATA_H_
#define UGDK_SCRIPT_LUA_LUADATA_H_

#include <ugdk/script/virtualdata.h>
#include <ugdk/script/lua/header.h>
#include <ugdk/script/lua/gear.h>
#include <ugdk/script/lua/luawrapper.h>

namespace ugdk {
namespace script {
namespace lua {

class LuaData : public VirtualData, public Identifiable {

  public:

    LuaData(LuaWrapper* wrapper) : wrapper_(wrapper) {}

    ~LuaData() {}

    void* Unwrap(const VirtualType& type) const;

    void Wrap(void* data, const VirtualType& type);
    void Wrap(const char* str);
    void Wrap(bool boolean);
    void Wrap(int number);
    void Wrap(double number);

    LangWrapper* wrapper () const { return wrapper_; }

    Ptr Execute(std::vector<Ptr> args);

    Ptr GetAttribute(Ptr key);

    Ptr SetAttribute(Ptr key, Ptr value);

  private:

    LuaWrapper* wrapper_;

    //DataID id() { return static_cast<void*>(this); }

};

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */

#endif /* UGDK_SCRIPT_LUA_LUADATA_H_ */
