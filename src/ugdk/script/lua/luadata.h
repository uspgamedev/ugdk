
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
    void Wrap(const char* str) { GenericWrap(str); }
    void Wrap(bool boolean) { GenericWrap(boolean); }
    void Wrap(int number) { GenericWrap(number); }
    void Wrap(double number) { GenericWrap(number); }

    LangWrapper* wrapper () const { return wrapper_; }

    Ptr Execute(const std::vector<Ptr>& args);

    Ptr GetAttribute(Ptr key);

    Ptr SetAttribute(Ptr key, Ptr value);

  //protected:

    void AddToBuffer();

  private:

    LuaWrapper* wrapper_;

    template <class T>
    void GenericWrap(T data) {
        Gear g(wrapper_->MakeGear());
        g->push<T>(data); // TODO TEST THIS!
        g.SetData(id());
    }

};

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */

#endif /* UGDK_SCRIPT_LUA_LUADATA_H_ */
