
#ifndef UGDK_SCRIPT_LUA_BOOTSTRAPGEAR_H_
#define UGDK_SCRIPT_LUA_BOOTSTRAPGEAR_H_

#include <vector>

#include <ugdk/script/lua/basegear.h>
#include <ugdk/script/lua/header.h>
#include <ugdk/script/lua/state.h>

namespace ugdk {
namespace script {
namespace lua {

class BootstrapGear : public BaseGear {

  public:

    BootstrapGear(lua_State* L) :
        BaseGear(L) {}

    ~BootstrapGear() {}

    bool Initialize(const std::vector<Module>& modules);

    DataID GenerateDatatable();

    void DestroyDatatable(DataID datable_id);

  private:

    static int SafeInitialize(lua_State* L);

    void LoadLibs();

    void PreloadModules(const std::vector<Module>& modules);

};

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */

#endif /* UGDK_SCRIPT_LUA_BOOTSTRAPGEAR_H_ */
