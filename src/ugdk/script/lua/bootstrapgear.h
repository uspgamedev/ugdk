
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

    void LoadLibs ();

    void PreloadModule(const std::vector<Module>& modules);

    DataID GenerateDatatable();

    void DestroyDatatable(DataID datable_id);

};

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */

#endif /* UGDK_SCRIPT_LUA_BOOTSTRAPGEAR_H_ */
