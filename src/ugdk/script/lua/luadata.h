
#ifndef UGDK_SCRIPT_LUA_LUADATA_H_
#define UGDK_SCRIPT_LUA_LUADATA_H_

#include <ugdk/script/virtualdata.h>

namespace ugdk {
namespace script {
namespace lua {

class LuaData: public VirtualData {

  public:

    LuaData() {}
    ~LuaData() {}

    bool Initialize();

};

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */

#endif /* UGDK_SCRIPT_LUA_LUADATA_H_ */
